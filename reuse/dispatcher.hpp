//
// Created by Luoxs on 2016-03-16
// Last modification on 2018-02-02
//
#ifndef __reuse_dispatcher_hpp__
#define __reuse_dispatcher_hpp__

namespace reuse {

template <typename Key, typename Handler, typename Hash = std::hash<Key>, typename KeyEqual = std::equal_to<Key>>
class dispatcher : private std::unordered_map<Key, Handler, Hash, KeyEqual>
{
public:
    void register_handler(const key_type& keyval, const mapped_type& mapval)
    {
        emplace(value_type(keyval, mapval));
    }

    void register_handler(key_type&& keyval, mapped_type&& mapval)
    {
        emplace(std::forward<key_type>(keyval), std::forward<mapped_type>(mapval));
    }

    template <typename... Arg>
    void delivery(const key_type& keyval, Arg&&... arg)
    {
        auto&& fn = find(keyval);
        if (fn != end())
            fn->second(std::forward<Arg>(arg)...);
    }
};

} // namespace reuse

#endif //__reuse_dispatcher_hpp__
