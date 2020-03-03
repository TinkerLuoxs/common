//
// Created by Luoxs on 2017-04-19
//
#pragma once

#include <string>
#include <vector>
#include <memory>
#include <deque>
#include <map>

typedef unsigned char           t_uint8;
typedef unsigned short          t_uint16;
typedef unsigned int            t_uint32;
typedef unsigned long long      t_uint64;
typedef std::string             t_string;
typedef std::shared_ptr<void>   t_buffer;


template <typename T>
using t_vector = std::vector<T>;

template <size_t N>
struct t_array
{
    char elems[N + 1];
    char *data()
    {
        return elems;
    }
    size_t size()
    {
        return N;
    }
    char& operator[](size_t pos)
    {
        return elems[pos];
    }
};

template <typename T>
using t_deque = std::deque<T>;

template <typename K, typename T, typename P>
using t_map = std::map<K, T, P>;