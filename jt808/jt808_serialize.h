//
// Created by Luoxs on 2017-04-19
//
#pragma once

#include "jt808_archive.h"

namespace jt
{

static inline t_uint8 calc_jt_t_check_code(const t_uint8 *buf, size_t size)
{
    t_uint8 check_code = *buf++;
    while (--size > 0)
        check_code ^= *buf++;
    return check_code;
}

class encoder
{
public:
    struct buffer_type
    {
        t_buffer data;
        size_t   size;
    };

    template <typename T>
    buffer_type to_buffer(T const& t, t_string const& phone)
    {
        head808_privated head = {};
        head.msgID = T::msgID;
        head.msgSeq = ++_auto_series;
        memset(head.phone.data(), '0', 12);
        int size = phone.size() >= 12 ? 0 : 12 - phone.size();
        strncpy(&head.phone[size], phone.data(), 12);

        oarchive obj_oar;
        const_cast<T&>(t).serialize(obj_oar);

        t_uint32 obj_size = obj_oar.size();

        if (obj_size > 0x03ff)
        {
            head.packetTotal = (t_uint16)((obj_size + 0x03fe) / 0x03ff);
            head.packetSeq = 1;
            head.msgAttr = 0x2000;
        }

        const t_uint8 *obj_buf = obj_oar.data();

        int i = head.packetTotal;
        size_t sub_obj_size = 0;
        t_uint8 check_code = 0;

        oarchive final_oar;
        final_oar.reserve(obj_size + 32 * (head.packetTotal + 1));

        const t_uint8 esc = 0x7e;
        do
        {
            _oarchive.clear();

            final_oar(esc);

            if (obj_size > 0x03ff)
            {
                sub_obj_size = 0x03ff;
                obj_size -= 0x03ff;
            }
            else
            {
                sub_obj_size = obj_size;
            }

            head.msgAttr |= sub_obj_size;
            head.serialize(_oarchive);
            head.msgAttr = 0x2000;
            ++head.packetSeq;

            _oarchive.copy(obj_buf, sub_obj_size);
            obj_buf += sub_obj_size;
            check_code = calc_jt_t_check_code(_oarchive.data(), _oarchive.size());
            _oarchive(check_code);

            _oarchive.escape_buffer(final_oar, 0);

            final_oar(esc);

        } while (--i > 0);

        buffer_type buf;
        buf.size = final_oar.size();
        buf.data.reset(final_oar.release(), ::free);
        return buf;
    }

private:
    oarchive _oarchive;
    t_uint16 _auto_series = 0;
};

struct packet_key
{
    t_uint16 msgID;
    t_uint16 msgSeq;

    packet_key(t_uint16 id, t_uint16 seq) : msgID(id), msgSeq(seq) {}
};

struct packet_key_less
{
    bool operator()(const packet_key& lhs, const packet_key& rhs) const
    {
        if (lhs.msgID < rhs.msgID)
            return true;
        else if (lhs.msgID == rhs.msgID && lhs.msgSeq < rhs.msgSeq)
            return true;
        return false;
    }
};

struct packet_value
{
    t_uint32 size;
    t_uint8  data[1024];

    packet_value(const void *d, t_uint32 s) : size(s)
    {
        memcpy(data, d, size);
    }
};

enum decode_type
{
    PARSE_PACKET_COMPLETED		= 0,
    PARSE_PACKET_DATA_LACK		= 1,
    PARSE_PACKET_DATA_LOSE		= 2,
    PARSE_PACKET_PROTOCOL_ERR	= 3
};

class decoder
{
public:
    //@step 1.
    decode_type packet_parse(const t_uint8 *buf, size_t buf_size)
    {
        _size_consumed = 0;
        if (!buf || !buf_size)
            return PARSE_PACKET_DATA_LACK;

        if (*buf != 0x7e)
            return PARSE_PACKET_PROTOCOL_ERR;

        _size_consumed = 1;

        decode_type result = content_parse(buf + 1, buf_size - 1);

        if (result != PARSE_PACKET_COMPLETED)
            return result;

        _size_cache -= 1; //exclude check_code character
        if (_data_cache[_size_cache] != calc_jt_t_check_code(_data_cache, _size_cache))
        {
            REUSE_DEBUG("check_code verify mismatch, data lose!\r\n");
            return PARSE_PACKET_DATA_LOSE;
        }

        _iarchive.set_buf(_data_cache, _size_cache);
        _header.serialize(_iarchive);

        if ((_header.msgAttr & 0x03ff) != _iarchive.size())
        {
            REUSE_DEBUG("body size verify mismatch, data lose\r\n");
            return PARSE_PACKET_DATA_LOSE;
        }

        if (_header.packetTotal > 0)
        {
            _subpackets.emplace(std::piecewise_construct,
                std::forward_as_tuple(_header.msgID, _header.packetSeq),
                std::forward_as_tuple(_iarchive.data(), _iarchive.size()));

            t_uint16 in_num = 0;
            auto it = _subpackets.lower_bound(packet_key(_header.msgID, 0));
            for (; it != _subpackets.end(); it++)
            {
                if (it->first.msgID == _header.msgID)
                    ++in_num;
                else
                    break;
            }

            if (_header.packetTotal == in_num)
            {
                _completed = true;
                return PARSE_PACKET_COMPLETED;
            }
            else
                return PARSE_PACKET_DATA_LACK;
        }
        _completed = false;
        return PARSE_PACKET_COMPLETED;
    }

    //@step 2.
    size_t size_consumed()
    {
        return _size_consumed;
    }

    //@step 2.
    //It's require to complete invoke $step 1.$
    head808 get_header()
    {
        head808 head;
        head.msgID = _header.msgID;
        head.msgSeq = _header.msgSeq;
        auto str = _header.phone.data();
        while (*str == '0') ++str;
        head.phone = str;
        return std::move(head);
    }

    //@step 3.
    //It's require to complete invoke $step 2.$
    template <typename T>
    bool get(T& t)
    {
        if (_header.msgID != T::msgID)
            return false;

        if (_completed)
        {
            std::deque<const_buffer> bufs;
            auto end = _subpackets.lower_bound(packet_key(_header.msgID, 0));
            auto begin = end;

            for (; end != _subpackets.end(); ++end)
            {
                if (end->first.msgID != _header.msgID)
                    break;
                bufs.push_back(const_buffer(end->second.data, end->second.size));
            }

            _iarchive.set_buf(std::move(bufs));

            t.serialize(_iarchive);

            while (begin != end)
                _subpackets.erase(begin++);
        }
        else
            t.serialize(_iarchive);

        return _iarchive.is_ok;
    }

private:
    decode_type content_parse(const t_uint8 *src, size_t src_size)
    {
        const t_uint8 *iter = src;
        bool completed = false;
        _size_cache = 0;

        for (; src_size-- && _size_cache < sizeof(_data_cache); ++_size_cache)
        {
            if (*iter == 0x7e)
            {
                ++iter;
                completed = true;
                break;
            }
            else if (*iter == 0x7d)
            {
                ++iter;
                if (src_size == 0)
                {
                    _size_consumed = 0;
                    return PARSE_PACKET_DATA_LACK;
                }
                --src_size;

                switch (*iter)
                {
                case 0x01:
                    _data_cache[_size_cache] = 0x7d;
                    break;
                case 0x02:
                    _data_cache[_size_cache] = 0x7e;
                    break;
                default:
                    REUSE_DEBUG("escape character format error: 0x%02x follow 0x7d\r\n", *iter);
                    ++iter;
                    _size_consumed += iter - src;
                    return PARSE_PACKET_PROTOCOL_ERR;
                }
                ++iter;
            }
            else
            {
                _data_cache[_size_cache] = *iter;
                ++iter;
            }
        }

        _size_consumed += iter - src;

        if (completed)
        {
            if (_size_cache < 12)
            {
                REUSE_DEBUG("packet size[%d] less than minimum data size[12]!\r\n", _size_cache);
                return PARSE_PACKET_PROTOCOL_ERR;
            }
            return PARSE_PACKET_COMPLETED;
        }

        if (_size_cache >= sizeof(_data_cache))
        {
            REUSE_DEBUG("packet size[%d] more than maximum data size[1040]!\r\n", _size_cache);
            return PARSE_PACKET_PROTOCOL_ERR;
        }
        _size_consumed = 0;
        return PARSE_PACKET_DATA_LACK;
    }

    bool _completed = false;
    size_t _size_consumed;
    size_t _size_cache;
    t_uint8 _data_cache[1040];
    iarchive _iarchive;
    head808_privated _header;
    t_map<packet_key, packet_value, packet_key_less> _subpackets;
};


}