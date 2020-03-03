//
// Created by Luoxs on 2016-03-16
// Last modification on 2018-02-02
//
#ifndef __reuse_binary_serialize_hpp__
#define __reuse_binary_serialize_hpp__

namespace reuse {

/// control extractions from a stream buffer
class binary_iarchive : public reuse::buffer
{
public:
    binary_iarchive(reuse::buffer& buf) : buffer(buf) {}

    template<typename T>
    void operator&(T& val)
    {
        *this >> val;
    }

    template <typename T, typename = std::enable_if<std::is_arithmetic<T>::value>::type>
    void operator>>(T& val)
    {
        if (pNext - pBegin < sizeof(T))
            is_ok = false;
        else {
            memcpy(&val, pBegin, sizeof(T));
            pBegin += sizeof(T);
        }
    }

    template <typename T, typename = std::enable_if<
        std::is_same<T, typename std::string<T::value_type>>::value
        || std::is_same<T, typename std::vector<T::value_type>>::value
        || std::is_same<T, typename std::list<T::value_type>>::value
        || std::is_same<T, typename std::deque<T::value_type>>::value>::type>
    void operator>>(T& val)
    {
        unsigned int size = 0;
        *this >> size;
        val.resize(size);
        for (auto& v : val)
            *this >> v;
    }

    template <typename T, typename = std::enable_if<std::is_class<T>::value>::type>
    void operator>>(T& val)
    {
        val.serialize(*this);
    }

    bool is_ok = true;
};

/// control insertions into a stream buffer
class binary_oarchive : public reuse::buffer
{
public:
    using reuse::buffer::buffer;

    template<typename T>
    void operator&(T const& val)
    {
        *this << val;
    }

    template <typename T, typename = std::enable_if<std::is_arithmetic<T>::value>::type>
    void operator<<(T const& val) 
    {
        if (!reserve(sizeof(T)))
            is_ok = false;
        else {
            memcpy(pNext, &val, sizeof(T));
            pNext += sizeof(T);
        }
    }

    template <typename T, typename = std::enable_if<
        std::is_same<T, typename std::string<T::value_type>>::value
        || std::is_same<T, typename std::vector<T::value_type>>::value 
        || std::is_same<T, typename std::list<T::value_type>>::value
        || std::is_same<T, typename std::deque<T::value_type>>::value>::type>
    void operator<<(T const& val)
    {
        unsigned int size = (unsigned int)val.size();
        *this << size;
        for (auto& v : val)
            *this << v;
    }

    template <typename T, typename = std::enable_if<std::is_class<T>::value>::type>
    void operator<<(T const& val)
    {
        val.serialize(const_cast<binary_oarchive&>(*this));
    }

    bool is_ok = true;
};

} // namespace reuse

#endif // __reuse_binary_serialize_hpp__
