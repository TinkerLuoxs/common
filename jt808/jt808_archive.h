//
// Created by Luoxs on 2017-04-19
//
#pragma once

#include "jt808_message.h"
#include "reuse/utility.h"
#include "serialize/archive/basic_iostream.hpp"


namespace jt
{

struct const_buffer
{
	const t_uint8 *head;
	const t_uint8 *tail;
    const_buffer(const t_uint8 *h, const t_uint8 *t) : head(h), tail(t) {}
	const_buffer(const t_uint8 *h, size_t s) : head(h), tail(h + s)	{}
};

class istream
{
public:
	void set_buf(const t_uint8 *buf, size_t size)
	{
		_buffers.clear();
		_buffers.emplace_back(buf, size);
		_next = _buffers.front().head;
	}

	void set_buf(std::deque<const_buffer> bufs)
	{
		_buffers = std::move(bufs);
		_next = _buffers.front().head;
	}

	t_uint32 size()
	{
		auto it = _buffers.begin();
		auto size = it->tail - _next;
		while (++it != _buffers.end())
			size += it->tail - it->head;
		return size;
	}

	const t_uint8 *data() const
	{
		return _next;
	}

	bool copy(void *data, size_t size)
	{
		size_t size_usable = 0;
		t_uint8 *pdata = (t_uint8 *)data;

		do
		{
			size_usable = _buffers.front().tail - _next;

			if (size_usable >= size)
			{
				memcpy(pdata, _next, size);
				_next += size;
				return true;
			}
			else
			{
				if (_buffers.size() == 1)
				{
					_next = _buffers.front().tail;
					return false;
				}

				memcpy(pdata, _next, size_usable);
				size -= size_usable;
				pdata += size_usable;

				_buffers.pop_front();
				_next = _buffers.front().head;
			}

		} while (size);

		return true;
	}

private:
	const t_uint8 *_next = nullptr;
	std::deque<const_buffer> _buffers;
};

class iarchive : public istream
{
public:
    bool is_ok = true;

	void operator()(t_uint32& val)
	{
        if (!copy(&val, sizeof(val)))
            is_ok = false;
        else
		    val = ntohl(val);
	}

	void operator()(t_uint16& val)
	{
        if (!copy(&val, sizeof(val)))
            is_ok = false;
        else
		    val = ntohs(val);
	}

	void operator()(t_uint8& val)
	{
        if (!copy(&val, sizeof(val)))
            is_ok = false;
	}

    void operator()(t_string& val)
    {
        if (!copy(&val[0], this->size()))
            is_ok = false;
        else
            val = reuse::codecvt<reuse::gbk, reuse::utf8>(val);
    }

    template <typename T>
	std::enable_if_t<std::is_integral<T>::value>
        operator()(t_string& val, T size)
	{
        operator()(size);
		val.resize(size + 1, 0);
        if (!copy(&val[0], size))
            is_ok = false;
        else
		    val = reuse::codecvt<reuse::gbk, reuse::utf8>(val);
	}

    template <size_t N>
    void operator()(t_array<N>& val)
    {
        if (!copy(val.data(), N - 1))
            is_ok = false;
    }

    template <size_t N>
    void operator()(t_array<N>& val, const void *)
    {
        constexpr size_t size = (N - 1) / 2;
        char buf[size] = { 0 };

        if (!copy(buf, size))
            is_ok = false;
        else
        {
            for (size_t i = 0, j = 0; j < size; ++j)
            {
                val[i++] = (0x0f & (buf[j] >> 4)) + '0';
                val[i++] = (0x0f & buf[j]) + '0';
            }
        }
    }

    void operator()(t_buffer& val, size_t size)
	{
        if (size == 0)
            return;

        val.reset(::malloc(size + 1), ::free);
        if (!val || !copy(val.get(), size))
            is_ok = false;
	}

    template <typename T>
    void operator()(t_vector<T>& val, size_t size)
    {
        val.resize(size);
        for (auto& t : val)
            operator()(t);
    }

    template <typename T>
    void operator()(t_vector<T>& val)
    {
        while (this->size() > 0)
        {
            val.emplace_back();
            operator()(val.back());
        }
    }

    template <typename T>
    void operator()(T& val)
    {
        val.serialize(*this);
    }
};

class oarchive : public serialize::archive::basic_ostream
{
public:
    void operator()(const t_uint32& val)
    {
        const t_uint32 tmp = htonl(val);
        copy(&tmp, sizeof(tmp));
    }

    void operator()(const t_uint16& val)
    {
        const t_uint16 tmp = htons(val);
        copy(&tmp, sizeof(tmp));
    }

    void operator()(const t_uint8& val)
    {
        copy(&val, sizeof(val));
    }

    void operator()(const t_string& val)
    {
        auto tmp = reuse::codecvt<reuse::utf8, reuse::gbk>(val);
        copy(tmp.data(), tmp.size());
    }

    template <typename T>
    std::enable_if_t<std::is_integral<T>::value>
        operator()(const t_string& val, T size)
    {
        auto tmp = reuse::codecvt<reuse::utf8, reuse::gbk>(val);
        size = static_cast<T>(tmp.size());
        operator()(size);
        copy(tmp.data(), size);
    }

    template <size_t N>
    void operator()(t_array<N>& val)
    {
        copy(val.data(), N - 1);
    }

    template <size_t N>
    void operator()(t_array<N>& val, const void *)
    {
        constexpr size_t size = (N - 1) / 2;
        char buf[size] = { 0 };
        const char *psrc = val.data();
        char tmp = 0;
        for (size_t i = 0; i < size; ++i)
        {
            tmp = (*psrc++ - '0') << 4;
            buf[i] = tmp | (*psrc++ - '0');
        }
        copy(buf, size);
    }

    void operator()(const t_buffer& val, size_t size)
    {
        copy(val.get(), size);
    }

    template <typename T>
    void operator()(const t_vector<T>& val)
    {
        for (auto& t : val)
            operator()(t);
    }

    template <typename T>
    void operator()(const t_vector<T>& val, size_t)
    {
        operator()(val);
    }

    template <typename T>
    void operator()(const T& val)
    {
        const_cast<T&>(val).serialize(*this);
    }

    void escape_buffer(oarchive& that, size_t pos)
    {
        static const t_uint8 esc_byte1[2] = { 0x7d, 0x01 };
        static const t_uint8 esc_byte2[2] = { 0x7d, 0x02 };

        const t_uint8 *iter = this->data() + pos;
        const t_uint8 *tail = this->data() + this->size();

        that.reserve(tail - iter);
        for (; iter < tail; ++iter)
        {
            switch (*iter)
            {
            default:
                that.copy(iter, 1);
                break;
            case 0x7e:
                that.copy(esc_byte2, 2);
                break;
            case 0x7d:
                that.copy(esc_byte1, 2);
                break;
            }
        }
    }
};


struct head808_privated
{
    t_uint16	msgID;
    t_uint16    msgAttr;
    t_uint16	msgSeq;
	t_uint16    packetSeq;
	t_uint16    packetTotal;
    t_array<12> phone;
    DECLARE_SERIALIZE();
};

template <typename Archive>
void head808_privated::serialize(Archive& ar)
{
    ar(msgID);
    ar(msgAttr);
    ar(phone, nullptr);
    ar(msgSeq);
    if ((0x2000 & msgAttr) == 0x2000)
    {
        ar(packetTotal);
        ar(packetSeq);
    }
}

template <typename Archive>
void dev_common_reply::serialize(Archive& ar)
{
    ar(reqSeq);
    ar(reqMsgID);
    ar(result);
}

template <typename Archive>
void serv_common_reply::serialize(Archive& ar)
{
    ar(reqSeq);
    ar(reqMsgID);
    ar(result);
}

template <typename Archive>
void heartbeat_req::serialize(Archive& ar)
{

}

template <typename Archive>
void register_req::serialize(Archive& ar)
{
    ar(provinceID);
    ar(cityID);
    ar(firmsID);
    ar(devTag);
    ar(devID);
    ar(carColor);	
    ar(carIdentity);
}

template <typename Archive>
void register_reply::serialize(Archive& ar)
{
    ar(reqSeq);
    ar(result);	
    ar(authCode);  
}

template <typename Archive>
void unregister_req::serialize(Archive& ar)
{

}

template <typename Archive>
void authenticate_req::serialize(Archive& ar)
{
    ar(authCode);
}

template <>
void parameter::serialize<iarchive>(iarchive& ar)
{
    t_uint8 value_length = 0;

    switch (id)
    {
    case PARAM_HEARTBEAT:
    case PARAM_TCP_TIMEOUT:
    case PARAM_TCP_RETRY:
    case PARAM_UDP_TIMEOUT:
    case PARAM_UDP_RETRY:
    case PARAM_SMS_TIMEOUT:
    case PARAM_SMS_RETRY:
    case PARAM_TCP_PORT:
    case PARAM_UDP_PORT:
    case PARAM_AUTH_HOST_TCP_PORT:
    case PARAM_AUTH_HOST_UDP_PORT:
    case PARAM_POSITION_STRATEGY:
    case PARAM_POSITION_SCHEME:
    case PARAM_DRIVER_NOLOGIN_TIME:
    case PARAM_DORMANT_TIME:
    case PARAM_URGENT_ALARM_TIME:
    case PARAM_DEFAULT_REPORT_TIME:
    case PARAM_DEFAULT_DISTANCE:
    case PARAM_DRIVER_NOLOGIN_DISTANCE:
    case PARAM_DORMANT_DISTANCE:
    case PARAM_URGENT_ALARM_DISTANCE:
    case PARAM_CURVE_ANGLE:
    case PARAM_DEV_ANSWER_TEL_STRATEGY:
    case PARAM_ONCE_MAX_CALL_TIME:
    case PARAM_MONTH_MAX_CALL_TIME:
    case PARAM_ALARM_HIDE:
    case PARAM_ALARM_SMS_SWITCH:
    case PARAM_ALARM_PHOTO_SWITCH:
    case PARAM_ALARM_PHOTO_STORE:
    case PARAM_MAJOR_ALARM:
    case PARAM_HIGHEST_SPEED:
    case PARAM_OVERSPEED_TIME:
    case PARAM_ONCE_DRIVE_THRESHOLD:
    case PARAM_DAY_DRIVE_THRESHOLD:
    case PARAM_MIN_REST_TIME:
    case PARAM_MAX_PARK_TIME:
    case PARAM_PHOTO_TIMING:
    case PARAM_PHOTO_DISTANCE:
    case PARAM_IMAGE_QUALITY:
    case PARAM_IMAGE_BRIGHTNESS:
    case PARAM_IMAGE_CONTRAST:
    case PARAM_IMAGE_SATURATION:
    case PARAM_IMAGE_CHROMA:
    case PARAM_MILEAGE:
    case PARAM_GNSS_SAMPLE_FREQUENCY:
    case PARAM_GNSS_UPLOAD_SETUP:
    case PARAM_CAN1_SAMPLE_TIME:
    case PARAM_CAN2_SAMPLE_TIME:
    {
        t_uint32 realvalue = 0;
        ar(value_length);
        ar(realvalue);
        value = std::to_string(realvalue);
        break;
    }
    case PARAM_FENCE_RADIUS:
    case PARAM_OVERSPEED_DEVIATION:
    case PARAM_FATIGUEDRIVE_DEVIATION:
    case PARAM_COLLIDE_ALARM:
    case PARAM_ROLLOVER_ALARM:
    case PARAM_PROVINCE_ID:
    case PARAM_CITY_ID:
    case PARAM_CAN1_UPLOAD_TIME:
    case PARAM_CAN2_UPLOAD_TIME:
    {
        t_uint16 realvalue = 0;
        ar(value_length);
        ar(realvalue);
        value = std::to_string(realvalue);
        break;
    }
    case PARAM_CARPALTE_COLOR:
    case PARAM_GNSS_POSITION:
    case PARAM_GNSS_BAUDRATE:
    case PARAM_GNSS_OUTPUT_FREQUENCY:
    case PARAM_GNSS_UPLOAD_MODE:
    {
        t_uint8 realvalue = 0;
        ar(value_length);
        ar(realvalue);
        value = std::to_string(realvalue);
        break;
    }
    case PARAM_HOST_DIAL_ACCESS:
    case PARAM_HOST_DIAL_USERNAME:
    case PARAM_HOST_DIAL_PASSWORD:
    case PARAM_HOST_ADDR:
    case PARAM_BACKUP_DIAL_ACCESS:
    case PARAM_BACKUP_DIAL_USERNAME:
    case PARAM_BACKUP_DIAL_PASSWORD:
    case PARAM_BACKUP_ADDR:
    case PARAM_AUTH_HOST_ADDR:
    case PARAM_AUTH_BACKUP_TCP_ADDR:
    case PARAM_MONITOR_SERV_TEL:
    case PARAM_RESET_TEL:
    case PARAM_DEFAULT_TEL:
    case PARAM_MONITOR_SERV_SMS_TEL:
    case PARAM_DEV_SMS_ALARM_TEL:
    case PARAM_LISTEN_TEL:
    case PARAM_PRIVILEGE_SMS_TEL:
    case PARAM_CARPLATE:
    {
        ar(value, value_length);
        break;
    }
    default:
        break;
    }
}

template <>
void parameter::serialize<oarchive>(oarchive& ar)
{
    t_uint8 value_length = 0;

    switch (id)
    {
    case PARAM_HEARTBEAT:
    case PARAM_TCP_TIMEOUT:
    case PARAM_TCP_RETRY:
    case PARAM_UDP_TIMEOUT:
    case PARAM_UDP_RETRY:
    case PARAM_SMS_TIMEOUT:
    case PARAM_SMS_RETRY:
    case PARAM_TCP_PORT:
    case PARAM_UDP_PORT:
    case PARAM_AUTH_HOST_TCP_PORT:
    case PARAM_AUTH_HOST_UDP_PORT:
    case PARAM_POSITION_STRATEGY:
    case PARAM_POSITION_SCHEME:
    case PARAM_DRIVER_NOLOGIN_TIME:
    case PARAM_DORMANT_TIME:
    case PARAM_URGENT_ALARM_TIME:
    case PARAM_DEFAULT_REPORT_TIME:
    case PARAM_DEFAULT_DISTANCE:
    case PARAM_DRIVER_NOLOGIN_DISTANCE:
    case PARAM_DORMANT_DISTANCE:
    case PARAM_URGENT_ALARM_DISTANCE:
    case PARAM_CURVE_ANGLE:
    case PARAM_DEV_ANSWER_TEL_STRATEGY:
    case PARAM_ONCE_MAX_CALL_TIME:
    case PARAM_MONTH_MAX_CALL_TIME:
    case PARAM_ALARM_HIDE:
    case PARAM_ALARM_SMS_SWITCH:
    case PARAM_ALARM_PHOTO_SWITCH:
    case PARAM_ALARM_PHOTO_STORE:
    case PARAM_MAJOR_ALARM:
    case PARAM_HIGHEST_SPEED:
    case PARAM_OVERSPEED_TIME:
    case PARAM_ONCE_DRIVE_THRESHOLD:
    case PARAM_DAY_DRIVE_THRESHOLD:
    case PARAM_MIN_REST_TIME:
    case PARAM_MAX_PARK_TIME:
    case PARAM_PHOTO_TIMING:
    case PARAM_PHOTO_DISTANCE:
    case PARAM_IMAGE_QUALITY:
    case PARAM_IMAGE_BRIGHTNESS:
    case PARAM_IMAGE_CONTRAST:
    case PARAM_IMAGE_SATURATION:
    case PARAM_IMAGE_CHROMA:
    case PARAM_MILEAGE:
    case PARAM_GNSS_SAMPLE_FREQUENCY:
    case PARAM_GNSS_UPLOAD_SETUP:
    case PARAM_CAN1_SAMPLE_TIME:
    case PARAM_CAN2_SAMPLE_TIME:
    {
        auto realvalue = (t_uint32)std::stoi(value);
        value_length = (t_uint8)sizeof(realvalue);
        ar(value_length);
        ar(realvalue);
        break;
    }
    case PARAM_FENCE_RADIUS:
    case PARAM_OVERSPEED_DEVIATION:
    case PARAM_FATIGUEDRIVE_DEVIATION:
    case PARAM_COLLIDE_ALARM:
    case PARAM_ROLLOVER_ALARM:
    case PARAM_PROVINCE_ID:
    case PARAM_CITY_ID:
    case PARAM_CAN1_UPLOAD_TIME:
    case PARAM_CAN2_UPLOAD_TIME:
    {
        auto realvalue = (t_uint16)std::stoi(value);
        value_length = (t_uint8)sizeof(realvalue);
        ar(value_length);
        ar(realvalue);
        break;
    }
    case PARAM_CARPALTE_COLOR:
    case PARAM_GNSS_POSITION:
    case PARAM_GNSS_BAUDRATE:
    case PARAM_GNSS_OUTPUT_FREQUENCY:
    case PARAM_GNSS_UPLOAD_MODE:
    {
        auto realvalue = (t_uint8)std::stoi(value);
        value_length = (t_uint8)sizeof(realvalue);
        ar(value_length);
        ar(realvalue);
        break;
    }
    case PARAM_HOST_DIAL_ACCESS:
    case PARAM_HOST_DIAL_USERNAME:
    case PARAM_HOST_DIAL_PASSWORD:
    case PARAM_HOST_ADDR:
    case PARAM_BACKUP_DIAL_ACCESS:
    case PARAM_BACKUP_DIAL_USERNAME:
    case PARAM_BACKUP_DIAL_PASSWORD:
    case PARAM_BACKUP_ADDR:
    case PARAM_AUTH_HOST_ADDR:
    case PARAM_AUTH_BACKUP_TCP_ADDR:
    case PARAM_MONITOR_SERV_TEL:
    case PARAM_RESET_TEL:
    case PARAM_DEFAULT_TEL:
    case PARAM_MONITOR_SERV_SMS_TEL:
    case PARAM_DEV_SMS_ALARM_TEL:
    case PARAM_LISTEN_TEL:
    case PARAM_PRIVILEGE_SMS_TEL:
    case PARAM_CARPLATE:
    {
        ar(value, value_length);
        break;
    }
    default:
        break;
    }
}

template <typename Archive>
void set_param_req::serialize(Archive& ar)
{
    t_uint8 items_size = (t_uint8)params.size();
    ar(items_size);
    ar(params, items_size);
}

template <typename Archive>
void get_all_param_req::serialize(Archive& ar)
{
    
}

template <typename Archive>
void get_part_param_req::serialize(Archive& ar)
{
    t_uint8 count = (t_uint8)paramIDs.size();
    ar(count);
    ar(paramIDs, count);
}

template <typename Archive>
void get_param_reply::serialize(Archive& ar)
{
    t_uint8 items_size = (t_uint8)params.size();
    ar(reqSeq);
    ar(items_size);
    ar(params, items_size);
}

template <typename Archive>
void control_dev_req::serialize(Archive& ar)
{
    ar(cmdID);
    ar(cmdValue);
}

template <typename Archive>
void property::serialize(Archive& ar)
{
    t_uint8 strlength = 0;
    ar(devType);
    ar(firmsID);
    ar(devTag);
    ar(devID);
    ar(simICCID, nullptr);
    ar(hardware, strlength);
    ar(firmware, strlength);
    ar(gnssAttr);
    ar(linkAttr);
}

template <typename Archive>
void get_property_req::serialize(Archive& ar)
{

}

template <typename Archive>
void get_property_reply::serialize(Archive& ar)
{
    ar(attr);
}

template <typename Archive>
void upgrade_req::serialize(Archive& ar)
{
    t_uint8 strlength = 0;
    ar(type);
    ar(firmsID);
    ar(version, strlength);
    ar(packetSize);
    ar(packetData, packetSize);
}

template <typename Archive>
void upgrade_reply::serialize(Archive& ar)
{
    ar(type);
    ar(result);
}

template <>
void annex::serialize<iarchive>(iarchive& ar)
{
    t_uint8 annexLen = 0;
    ar(id);
    ar(annexLen);
    switch (id)
    {
    case GPS_ANNEX_MILEAGE:
    case GPS_ANNEX_CAR_SIGNAL_STATUS:
    case GPS_ANNEX_ANALOG_QUANTITY:
    {
        t_uint32 realvalue = 0;
        ar(realvalue);
        value = std::to_string(realvalue);
        break;
    }
    case GPS_ANNEX_OIL_VOLUME:
    case GPS_ANNEX_RECORD_SPEED:
    case GPS_ANNEX_ALARM_EVENTID:
    case GPS_ANNEX_IO_STATUS:
    {
        t_uint16 realvalue = 0;
        ar(realvalue);
        value = std::to_string(realvalue);
        break;
    }
    case GPS_ANNEX_WIRELESS_SIGNAL:
    case GPS_ANNEX_GNSS_SATELLITE:
    {
        t_uint8 realvalue = 0;
        ar(realvalue);
        value = std::to_string(realvalue);
        break;
    }
    case GPS_ANNEX_OVERSPEED:
    {
        annexed_overspeed as = {};
        as.serialize(ar);
        value = std::to_string(as.siteType);
        value += ";";
        value += std::to_string(as.areaID);
        break;
    }
    case GPS_ANNEX_AREA_ALRAM:
    {
        annexed_arealarm aa = {};
        aa.serialize(ar);
        value = std::to_string(aa.siteType);
        value += ";";
        value += std::to_string(aa.areaID);
        value += ";";
        value += std::to_string(aa.direction);
        break;
    }
    case GPS_ANNEX_TRAVEL_TIME:
    {
        annexed_traveltime at = {};
        at.serialize(ar);
        value = std::to_string(at.areaID);
        value += ";";
        value += std::to_string(at.travelTime);
        value += ";";
        value += std::to_string(at.result);
        break;
    }
    default:
    {
        if (annexLen == 0)
            return;

        t_vector<t_uint8> drop(annexLen);
        ar(drop, annexLen);
#if defined(JT808_EXPAND_GPSANNEX)
        value = JT808_EXPAND_GPSANNEX_IARCHIVE(id, drop);
#else
        REUSE_DEBUG("nonrecognition gps annex id:0x%02x\r\n", id);
#endif
        break;
    }

    }

}

template <>
void annex::serialize<oarchive>(oarchive& ar)
{
    t_uint8 annexLen = 0;
    ar(id);
    switch (id)
    {
    case GPS_ANNEX_MILEAGE:
    case GPS_ANNEX_CAR_SIGNAL_STATUS:
    case GPS_ANNEX_ANALOG_QUANTITY:
    {
        auto realvalue = (t_uint32)std::stoi(value);
        annexLen = (t_uint8)sizeof(realvalue);
        ar(annexLen);
        ar(realvalue);
        break;
    }
    case GPS_ANNEX_OIL_VOLUME:
    case GPS_ANNEX_RECORD_SPEED:
    case GPS_ANNEX_ALARM_EVENTID:
    case GPS_ANNEX_IO_STATUS:
    {
        auto realvalue = (t_uint16)std::stoi(value);
        annexLen = (t_uint8)sizeof(realvalue);
        ar(annexLen);
        ar(realvalue);
        break;
    }
    case GPS_ANNEX_WIRELESS_SIGNAL:
    case GPS_ANNEX_GNSS_SATELLITE:
    {
        auto realvalue = (t_uint8)std::stoi(value);
        annexLen = (t_uint8)sizeof(realvalue);
        ar(annexLen);
        ar(realvalue);
        break;
    }
    case GPS_ANNEX_OVERSPEED:
    {
        annexed_overspeed as = {};
        sscanf(value.data(), "%hhu;%u", &as.siteType, &as.areaID);
        annexLen = as.siteType ? 5 : 1;
        ar(annexLen);
        as.serialize(ar);
        break;
    }
    case GPS_ANNEX_AREA_ALRAM:
    {
        annexed_arealarm aa = {};
        sscanf(value.data(), "%hhu;%u;%hhu", &aa.siteType, &aa.areaID, &aa.direction);
        annexLen = 6;
        ar(annexLen);
        aa.serialize(ar);
        break;
    }
    case GPS_ANNEX_TRAVEL_TIME:
    {
        annexed_traveltime at = {};
        sscanf(value.data(), "%u;%hu;%hhu", &at.areaID, &at.travelTime, &at.result);
        annexLen = 7;
        ar(annexLen);
        at.serialize(ar);
        break;
    }
    default:
    {
#if defined(JT808_EXPAND_GPSANNEX)
        auto data = JT808_EXPAND_GPSANNEX_OARCHIVE(this, annexLen);
        ar(annexLen);
        ar(data, annexLen);
#else
        annexLen = 0;
        ar(annexLen);
        REUSE_DEBUG("nonrecognition gps annex id:0x%02x\r\n", id);
#endif
        break;
    }

    }

}

template <>
void gps::serialize<iarchive>(iarchive& ar)
{
    t_uint32 lat = 0, lng = 0;
    t_array<12> bcdtime = {};

    ar(alarmType);
    ar(status);
    ar(lat);
    ar(lng);
    ar(elevation);
    ar(speed);
    ar(direction);
    ar(bcdtime, nullptr);
    ar(annexs);

    latitude = (double)lat / 1000000;
    longitude = (double)lng / 1000000;
    time.resize(20);
    sprintf(&time[0], "20%c%c-%c%c-%c%c %c%c:%c%c:%c%c", bcdtime[0], bcdtime[1],
        bcdtime[2], bcdtime[3], bcdtime[4], bcdtime[5], bcdtime[6], bcdtime[7],
        bcdtime[8], bcdtime[9], bcdtime[10], bcdtime[11]);
}

template <>
void gps::serialize<oarchive>(oarchive& ar)
{
    t_uint32 lat = t_uint32(latitude * 1000000);
    t_uint32 lng = t_uint32(longitude * 1000000);
    t_array<12> bcdtime = {};
    sscanf(time.data(), "20%c%c-%c%c-%c%c %c%c:%c%c:%c%c", &bcdtime[0], &bcdtime[1],
        &bcdtime[2], &bcdtime[3], &bcdtime[4], &bcdtime[5], &bcdtime[6], &bcdtime[7],
        &bcdtime[8], &bcdtime[9], &bcdtime[10], &bcdtime[11]);

    ar(alarmType);
    ar(status);
    ar(lat);
    ar(lng);
    ar(elevation);
    ar(speed);
    ar(direction);
    ar(bcdtime, nullptr);
    ar(annexs);
}

template <typename Archive>
void annexed_overspeed::serialize(Archive& ar)
{
    ar(siteType);
    if (siteType)
        ar(areaID);
}

template <typename Archive>
void annexed_arealarm::serialize(Archive& ar)
{
    ar(siteType);
    ar(areaID);
    ar(direction);
}

template <typename Archive>
void annexed_traveltime::serialize(Archive& ar)
{
    ar(areaID);
    ar(travelTime);
    ar(result);
}

template <typename Archive>
void report_gps::serialize(Archive& ar)
{
    ar(site);
}

template <typename Archive>
void get_gps_req::serialize(Archive& ar)
{

}

template <typename Archive>
void get_gps_reply::serialize(Archive& ar)
{
    ar(reqSeq);
    ar(site);
}

template <typename Archive>
void trace_gps_req::serialize(Archive& ar)
{
    ar(interval);
    ar(expiry);
}

template <typename Archive>
void verify_alarm::serialize(Archive& ar)
{
    ar(reqSeq);
    ar(alarmType);
}

template <typename Archive>
void notify_req::serialize(Archive& ar)
{
    ar(textFlag);
    ar(text);
}

template <typename Archive>
void get_driver_req::serialize(Archive& ar)
{

}

template <>
void get_driver_reply::serialize<iarchive>(iarchive& ar)
{
    t_array<12> bcdtime;
    t_array<8> bcdtime2;
    t_uint8 strlength = 0;

    ar(status);
    ar(bcdtime, nullptr);
    if (status == 0x01)
    {
        ar(flag);
        ar(name, strlength);
        ar(license);
        ar(licensor, strlength);
        ar(bcdtime2, nullptr);

        expiredTime.resize(20);
        sprintf(&expiredTime[0], "%c%c%c%c-%c%c-%c%c 00:00:00", bcdtime2[0], bcdtime2[1],
            bcdtime2[2], bcdtime2[3], bcdtime2[4], bcdtime2[5], bcdtime2[6], bcdtime2[7]);
    }

    time.resize(20);
    sprintf(&time[0], "20%c%c-%c%c-%c%c %c%c:%c%c:%c%c", bcdtime[0], bcdtime[1],
        bcdtime[2], bcdtime[3], bcdtime[4], bcdtime[5], bcdtime[6], bcdtime[7],
        bcdtime[8], bcdtime[9], bcdtime[10], bcdtime[11]);
}

template <>
void get_driver_reply::serialize<oarchive>(oarchive& ar)
{
    t_array<12> bcdtime;
    t_array<8> bcdtime2;
    t_uint8 strlength = 0;
    sscanf(time.data(), "20%c%c-%c%c-%c%c %c%c:%c%c:%c%c", &bcdtime[0], &bcdtime[1],
        &bcdtime[2], &bcdtime[3], &bcdtime[4], &bcdtime[5], &bcdtime[6], &bcdtime[7],
        &bcdtime[8], &bcdtime[9], &bcdtime[10], &bcdtime[11]);

    ar(status);
    ar(bcdtime, nullptr);
    if (status == 0x01)
    {
        ar(flag);
        ar(name, strlength);
        ar(license);
        ar(licensor, strlength);

        sscanf(expiredTime.data(), "%c%c%c%c-%c%c-%c%c 00:00:00", &bcdtime2[0], &bcdtime2[1],
            &bcdtime2[2], &bcdtime2[3], &bcdtime2[4], &bcdtime2[5], &bcdtime2[6], &bcdtime2[7]);

        ar(bcdtime2, nullptr);
    }
}


template <typename Archive>
void call_phone_req::serialize(Archive& ar)
{
    ar(flag);
    ar(phone);
}

template <typename Archive>
void set_record_req::serialize(Archive& ar)
{
    ar(cmd);
    ar(duration);
    ar(saveFlag);
    ar(sampleRate);
}

template <typename Archive>
void up_tunnel_data::serialize(Archive& ar)
{
    t_uint8 length = content.size();
    ar(type);
    ar(length);
    ar(content, length);
}

template <typename Archive>
void down_tunnel_data::serialize(Archive& ar)
{
    t_uint8 length = content.size();
    ar(type);
    ar(length);
    ar(content, length);
}

}