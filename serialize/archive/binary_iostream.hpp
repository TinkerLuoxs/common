//
// Created by Luoxs on 2017-03-16
//
#pragma once

#include "basic_iostream.hpp"

namespace serialize
{
namespace archive
{
namespace binary_size_calculated
{
    template <typename T, typename = std::enable_if_t<std::is_arithmetic<T>::value>>
    static void accumulate(T const&, size_t& size)
    {
        size += sizeof(T);
    }

    template <typename T>
    static std::enable_if_t<has_series<T>::value> accumulate(T const& t, size_t& size)
    {
        serialize::foreach([&size](auto&& elem){accumulate(elem, size); }, t);
    }

    template <typename T>
    static void accumulate(std::vector<T> const& t, size_t& size)
    {
        size += sizeof(unsigned int);
        for (auto& i : t)
            accumulate(i, size);
    }

    static void accumulate(std::string const& t, size_t& size)
    {
        size += sizeof(unsigned int);
        size += t.size();
    }

    template <typename T, size_t N>
    static void accumulate(T const (&t)[N], size_t& size)
    {
        size += N;
    }
}

    class binary_ostream : public basic_ostream
    {
    public:
        struct buffer_type
        {
            void *data;
            size_t size;
        };

        template <typename T>
        buffer_type to_bytes(T const& t)
        {
            size_t calc_size = 0;
            binary_size_calculated::accumulate(t, calc_size);
            reserve(calc_size);
            write(t);
            buffer_type buf = { data(), size() };
            return buf;
        }

    protected:
        template <typename T>
        std::enable_if_t<std::is_arithmetic<T>::value> write(T const& t)
        {
            copy(&t, sizeof(t));
        }

        template <typename T>
        std::enable_if_t<has_series<T>::value> write(T const& t)
        {
            serialize::foreach([this](auto&& elem) {this->write(elem); }, t);
        }

        template <typename T>
        void write(std::vector<T> const& t)
        {
            unsigned int size = t.size();
            copy(&size, sizeof(size));
            for (auto& i : t)
                write(i);
        }

        template <typename T, size_t N>
        void write(T const (&t)[N])
        {
            copy(t, N);
        }

        void write(std::string const& t)
        {
            unsigned int size = t.size();
            copy(&size, sizeof(size));
            copy(t.data(), size);
        }
    };

    class binary_istream : public basic_istream
    {
    public:
        template <typename T>
        bool from_bytes(T& t)
        {
            read(t);
            return is_ok;
        }

    protected:
        template <typename T>
        std::enable_if_t<std::is_arithmetic<T>::value> read(T &t)
        {
            copy(&t, sizeof(t));
        }

        template <typename T>
        std::enable_if_t<has_series<T>::value> read(T &t)
        {
            serialize::foreach([this](auto&& elem) {this->read(elem); }, t);
        }

        template <typename T>
        void read(std::vector<T>& t)
        {
            unsigned int size = 0;
            read(size);
            t.resize(size);
            for (auto& i : t)
                read(i);
        }

        template <typename T, size_t N>
        void read(T const (&t)[N])
        {
            copy(t, N);
        }

        void read(std::string& t)
        {
            unsigned int size = 0;
            read(size);
            t.resize(size);
            copy(&t[0], size);
        }
    };

}
}