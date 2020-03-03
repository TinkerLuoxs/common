//
// Created by Luoxs on 2017-03-16
//
#pragma once

namespace serialize
{
namespace archive
{
    class basic_istream
    {
    public:
        bool is_ok;

        void set_buf(unsigned char *buf, size_t size)
        {
            _ghead = _ghead = buf;
            _gtail = _ghead + size;
            is_ok = true;
        }

        bool copy(void *data, size_t size)
        {
            if (size > size_t(_gtail - _ghead))
                return (is_ok = false);
            memcpy(data, _ghead, size);
            _ghead += size;
            return true;
        }

        unsigned char* gptr()
        {
            return _ghead;
        }

    private:
        unsigned char *_ghead = nullptr;
        unsigned char *_gtail = nullptr;
    };

    class basic_ostream
    {
    public:
        basic_ostream() {}
        basic_ostream(basic_ostream const&) = delete;
        basic_ostream& operator=(basic_ostream const&) = delete;

        ~basic_ostream() { free(_phead); }

        bool copy(const void *data, size_t size)
        {
            if (!reserve(size))
                return false;
            memcpy(_ptail, data, size);
            _ptail += size;
            return true;
        }

        unsigned char* release()
        {
            auto p = _phead;
            _ptail = _phead = nullptr;
            _size_allocated = 0;
            return p;
        }

        void clear()
        {
            _ptail = _phead;
        }

        unsigned char* data() const
        {
            return _phead;
        }

        size_t size() const
        {
            return _ptail - _phead;
        }

        bool reserve(size_t size)
        {
            size_t size_used = _ptail - _phead;
            size_t size_usable = _size_allocated - size_used;
            if (size_usable > size)
                return true;

            size_t size_alter = ((size + op_alloc_size) / least_alloc_size) * least_alloc_size;
            size_alter += _size_allocated;

            auto pTem = (unsigned char *)malloc(size_alter);
            if (pTem == nullptr)
                return false;

            memcpy(pTem, _phead, size_used);
            free(_phead);
            _phead = pTem;
            _ptail = _phead + size_used;
            _size_allocated = size_alter;
            return true;
        }

    private:
        enum { least_alloc_size = 128, op_alloc_size = (least_alloc_size - 1) };

        unsigned char *_phead = nullptr;
        unsigned char *_ptail = nullptr;
        size_t _size_allocated = 0;
    };

}
}