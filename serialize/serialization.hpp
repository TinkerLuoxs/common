//
// Created by Luoxs on 2017-03-16
//
#pragma once

#include <tuple>

#define SERIES_CAT(a, b) SERIES_CAT_I(a, b)
#define SERIES_CAT_I(a, b) a ## b

#define SERIES_EXPAND(...) __VA_ARGS__

#define SERIES_FOR_1(macro, data, elem, ...)   macro(data, elem)
#define SERIES_FOR_2(macro, data, elem, ...)   macro(data, elem), SERIES_EXPAND(SERIES_FOR_1(macro, data, __VA_ARGS__))
#define SERIES_FOR_3(macro, data, elem, ...)   macro(data, elem), SERIES_EXPAND(SERIES_FOR_2(macro, data, __VA_ARGS__))
#define SERIES_FOR_4(macro, data, elem, ...)   macro(data, elem), SERIES_EXPAND(SERIES_FOR_3(macro, data, __VA_ARGS__))
#define SERIES_FOR_5(macro, data, elem, ...)   macro(data, elem), SERIES_EXPAND(SERIES_FOR_4(macro, data, __VA_ARGS__))
#define SERIES_FOR_6(macro, data, elem, ...)   macro(data, elem), SERIES_EXPAND(SERIES_FOR_5(macro, data, __VA_ARGS__))
#define SERIES_FOR_7(macro, data, elem, ...)   macro(data, elem), SERIES_EXPAND(SERIES_FOR_6(macro, data, __VA_ARGS__))
#define SERIES_FOR_8(macro, data, elem, ...)   macro(data, elem), SERIES_EXPAND(SERIES_FOR_7(macro, data, __VA_ARGS__))
#define SERIES_FOR_9(macro, data, elem, ...)   macro(data, elem), SERIES_EXPAND(SERIES_FOR_8(macro, data, __VA_ARGS__))
#define SERIES_FOR_10(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_9(macro, data, __VA_ARGS__))
#define SERIES_FOR_11(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_10(macro, data, __VA_ARGS__))
#define SERIES_FOR_12(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_11(macro, data, __VA_ARGS__))
#define SERIES_FOR_13(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_12(macro, data, __VA_ARGS__))
#define SERIES_FOR_14(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_13(macro, data, __VA_ARGS__))
#define SERIES_FOR_15(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_14(macro, data, __VA_ARGS__))
#define SERIES_FOR_16(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_15(macro, data, __VA_ARGS__))
#define SERIES_FOR_17(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_16(macro, data, __VA_ARGS__))
#define SERIES_FOR_18(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_17(macro, data, __VA_ARGS__))
#define SERIES_FOR_19(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_18(macro, data, __VA_ARGS__))
#define SERIES_FOR_20(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_19(macro, data, __VA_ARGS__))
#define SERIES_FOR_21(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_20(macro, data, __VA_ARGS__))
#define SERIES_FOR_22(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_21(macro, data, __VA_ARGS__))
#define SERIES_FOR_23(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_22(macro, data, __VA_ARGS__))
#define SERIES_FOR_24(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_23(macro, data, __VA_ARGS__))
#define SERIES_FOR_25(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_24(macro, data, __VA_ARGS__))
#define SERIES_FOR_26(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_25(macro, data, __VA_ARGS__))
#define SERIES_FOR_27(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_26(macro, data, __VA_ARGS__))
#define SERIES_FOR_28(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_27(macro, data, __VA_ARGS__))
#define SERIES_FOR_29(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_28(macro, data, __VA_ARGS__))
#define SERIES_FOR_30(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_29(macro, data, __VA_ARGS__))
#define SERIES_FOR_31(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_30(macro, data, __VA_ARGS__))
#define SERIES_FOR_32(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_31(macro, data, __VA_ARGS__))
#define SERIES_FOR_33(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_32(macro, data, __VA_ARGS__))
#define SERIES_FOR_34(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_33(macro, data, __VA_ARGS__))
#define SERIES_FOR_35(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_34(macro, data, __VA_ARGS__))
#define SERIES_FOR_36(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_35(macro, data, __VA_ARGS__))
#define SERIES_FOR_37(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_36(macro, data, __VA_ARGS__))
#define SERIES_FOR_38(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_37(macro, data, __VA_ARGS__))
#define SERIES_FOR_39(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_38(macro, data, __VA_ARGS__))
#define SERIES_FOR_40(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_39(macro, data, __VA_ARGS__))
#define SERIES_FOR_41(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_40(macro, data, __VA_ARGS__))
#define SERIES_FOR_42(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_41(macro, data, __VA_ARGS__))
#define SERIES_FOR_43(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_42(macro, data, __VA_ARGS__))
#define SERIES_FOR_44(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_43(macro, data, __VA_ARGS__))
#define SERIES_FOR_45(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_44(macro, data, __VA_ARGS__))
#define SERIES_FOR_46(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_45(macro, data, __VA_ARGS__))
#define SERIES_FOR_47(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_46(macro, data, __VA_ARGS__))
#define SERIES_FOR_48(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_47(macro, data, __VA_ARGS__))
#define SERIES_FOR_49(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_48(macro, data, __VA_ARGS__))
#define SERIES_FOR_50(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_49(macro, data, __VA_ARGS__))
#define SERIES_FOR_51(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_50(macro, data, __VA_ARGS__))
#define SERIES_FOR_52(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_51(macro, data, __VA_ARGS__))
#define SERIES_FOR_53(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_52(macro, data, __VA_ARGS__))
#define SERIES_FOR_54(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_53(macro, data, __VA_ARGS__))
#define SERIES_FOR_55(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_54(macro, data, __VA_ARGS__))
#define SERIES_FOR_56(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_55(macro, data, __VA_ARGS__))
#define SERIES_FOR_57(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_56(macro, data, __VA_ARGS__))
#define SERIES_FOR_58(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_57(macro, data, __VA_ARGS__))
#define SERIES_FOR_59(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_58(macro, data, __VA_ARGS__))
#define SERIES_FOR_60(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_59(macro, data, __VA_ARGS__))
#define SERIES_FOR_61(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_60(macro, data, __VA_ARGS__))
#define SERIES_FOR_62(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_61(macro, data, __VA_ARGS__))
#define SERIES_FOR_63(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_62(macro, data, __VA_ARGS__))
#define SERIES_FOR_64(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_63(macro, data, __VA_ARGS__))
#define SERIES_FOR_65(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_64(macro, data, __VA_ARGS__))
#define SERIES_FOR_66(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_65(macro, data, __VA_ARGS__))
#define SERIES_FOR_67(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_66(macro, data, __VA_ARGS__))
#define SERIES_FOR_68(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_67(macro, data, __VA_ARGS__))
#define SERIES_FOR_69(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_68(macro, data, __VA_ARGS__))
#define SERIES_FOR_70(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_69(macro, data, __VA_ARGS__))
#define SERIES_FOR_71(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_70(macro, data, __VA_ARGS__))
#define SERIES_FOR_72(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_71(macro, data, __VA_ARGS__))
#define SERIES_FOR_73(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_72(macro, data, __VA_ARGS__))
#define SERIES_FOR_74(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_73(macro, data, __VA_ARGS__))
#define SERIES_FOR_75(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_74(macro, data, __VA_ARGS__))
#define SERIES_FOR_76(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_75(macro, data, __VA_ARGS__))
#define SERIES_FOR_77(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_76(macro, data, __VA_ARGS__))
#define SERIES_FOR_78(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_77(macro, data, __VA_ARGS__))
#define SERIES_FOR_79(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_78(macro, data, __VA_ARGS__))
#define SERIES_FOR_80(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_79(macro, data, __VA_ARGS__))
#define SERIES_FOR_81(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_80(macro, data, __VA_ARGS__))
#define SERIES_FOR_82(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_81(macro, data, __VA_ARGS__))
#define SERIES_FOR_83(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_82(macro, data, __VA_ARGS__))
#define SERIES_FOR_84(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_83(macro, data, __VA_ARGS__))
#define SERIES_FOR_85(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_84(macro, data, __VA_ARGS__))
#define SERIES_FOR_86(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_85(macro, data, __VA_ARGS__))
#define SERIES_FOR_87(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_86(macro, data, __VA_ARGS__))
#define SERIES_FOR_88(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_87(macro, data, __VA_ARGS__))
#define SERIES_FOR_89(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_88(macro, data, __VA_ARGS__))
#define SERIES_FOR_90(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_89(macro, data, __VA_ARGS__))
#define SERIES_FOR_91(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_90(macro, data, __VA_ARGS__))
#define SERIES_FOR_92(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_91(macro, data, __VA_ARGS__))
#define SERIES_FOR_93(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_92(macro, data, __VA_ARGS__))
#define SERIES_FOR_94(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_93(macro, data, __VA_ARGS__))
#define SERIES_FOR_95(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_94(macro, data, __VA_ARGS__))
#define SERIES_FOR_96(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_95(macro, data, __VA_ARGS__))
#define SERIES_FOR_97(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_96(macro, data, __VA_ARGS__))
#define SERIES_FOR_98(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_97(macro, data, __VA_ARGS__))
#define SERIES_FOR_99(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_98(macro, data, __VA_ARGS__))
#define SERIES_FOR_100(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_99(macro, data, __VA_ARGS__))
#define SERIES_FOR_101(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_100(macro, data, __VA_ARGS__))
#define SERIES_FOR_102(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_101(macro, data, __VA_ARGS__))
#define SERIES_FOR_103(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_102(macro, data, __VA_ARGS__))
#define SERIES_FOR_104(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_103(macro, data, __VA_ARGS__))
#define SERIES_FOR_105(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_104(macro, data, __VA_ARGS__))
#define SERIES_FOR_106(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_105(macro, data, __VA_ARGS__))
#define SERIES_FOR_107(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_106(macro, data, __VA_ARGS__))
#define SERIES_FOR_108(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_107(macro, data, __VA_ARGS__))
#define SERIES_FOR_109(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_108(macro, data, __VA_ARGS__))
#define SERIES_FOR_110(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_109(macro, data, __VA_ARGS__))
#define SERIES_FOR_111(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_110(macro, data, __VA_ARGS__))
#define SERIES_FOR_112(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_111(macro, data, __VA_ARGS__))
#define SERIES_FOR_113(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_112(macro, data, __VA_ARGS__))
#define SERIES_FOR_114(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_113(macro, data, __VA_ARGS__))
#define SERIES_FOR_115(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_114(macro, data, __VA_ARGS__))
#define SERIES_FOR_116(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_115(macro, data, __VA_ARGS__))
#define SERIES_FOR_117(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_116(macro, data, __VA_ARGS__))
#define SERIES_FOR_118(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_117(macro, data, __VA_ARGS__))
#define SERIES_FOR_119(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_118(macro, data, __VA_ARGS__))
#define SERIES_FOR_120(macro, data, elem, ...)  macro(data, elem), SERIES_EXPAND(SERIES_FOR_119(macro, data, __VA_ARGS__))


#define SERIES_SIZE(...) SERIES_CAT(SERIES_SIZE_I(__VA_ARGS__, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110,\
        109, 108, 107, 106, 105, 104, 103, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85,\
        84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57,\
        56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, \
        28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0),)
#define SERIES_SIZE_I(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21,\
        e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44,\
        e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63, e64, e65, e66, e67,\
        e68, e69, e70, e71, e72, e73, e74, e75, e76, e77, e78, e79, e80, e81, e82, e83, e84, e85, e86, e87, e88, e89, e90,\
        e91, e92, e93, e94, e95, e96, e97, e98, e99, e100, e101, e102, e103, e104, e105, e106, e107, e108, e109, e110, e111,\
        e112, e113, e114, e115, e116, e117, e118, e119, size, ...) size


#define SERIES_FOR_EACH(N, macro, data, elem, ...) \
    SERIES_CAT(SERIES_FOR_ , N)(macro, data, elem, __VA_ARGS__)

template <typename T>
struct __series {};

template <typename T>
using __remove_cvr_t = typename std::remove_cv<typename std::remove_reference<T>::type>::type;

template <typename... T>
struct __make_void
{
    typedef void type;
};

template <typename T, typename = void>
struct has_series : std::false_type
{
};

template <typename T>
struct has_series<T, typename __make_void<typename __series<__remove_cvr_t<T>>::type>::type>
    : std::true_type
{
};

#define SERIES_META(a, b) &a::b

#define SERIES_TEXT(_, b) #b

#define SERIES_MAKE_META(S, N, ...) \
template <>struct __series<S> {\
    constexpr static decltype(auto) apply() {\
        return std::make_tuple(SERIES_EXPAND(SERIES_FOR_EACH(N, SERIES_META, S, __VA_ARGS__)));\
    }\
    typedef void type;\
    constexpr static size_t size = N;\
};

#define SERIES_MAKE_META_X(S, N, ...) \
template <>struct __series<S> {\
    constexpr static decltype(auto) apply() {\
        return std::make_tuple(SERIES_EXPAND(SERIES_FOR_EACH(N, SERIES_META, S, __VA_ARGS__)));\
    }\
    typedef void type;\
    constexpr static size_t size = N;\
    constexpr static const char * const name = #S;\
    constexpr static const char * const mem[N] = {SERIES_EXPAND(SERIES_FOR_EACH(N, SERIES_TEXT, _, __VA_ARGS__))};\
};

// structure serialize, only sequence apply to embody
#define SERIALIZE(STRUCTURE, ...) \
    SERIES_MAKE_META(STRUCTURE, SERIES_SIZE(__VA_ARGS__), __VA_ARGS__)

// structure serialize, sequence and name apply to embody
#define SERIALIZE_X(STRUCTURE, ...) \
    SERIES_MAKE_META_X(STRUCTURE, SERIES_SIZE(__VA_ARGS__), __VA_ARGS__)

// It's required when structure member is privated
#define FRIEND_SERIALIZE(STRUCTURE) friend struct __series<STRUCTURE>;

// It's required when structure member is empty
#define EMPTY_SERIALIZE(STRUCTRUE) \
template <>struct __series<STRUCTRUE> {\
    constexpr static decltype(auto) apply() {\
        return std::make_tuple();\
    }\
    typedef void type;\
    constexpr static size_t size = 0;\
};

namespace serialize
{

template <typename F, typename T>
constexpr void invoke(F&& f, T&& t)
{
    std::forward<F>(f)(std::forward<T>(t));
}

template <typename F, typename T>
constexpr void invoke(F&& f, T&& t, const char * const name)
{
    std::forward<F>(f)(std::forward<T>(t), name);
}

template <typename F, typename T, typename Memfn, size_t... I>
constexpr void foreach_impl(F&& f, T&& t, Memfn&& mfn, std::index_sequence<I...>)
{
    using swallow = int[];
    (void)swallow {
        0, (void(invoke(std::forward<F>(f), std::forward<T>(t).*std::get<I>(mfn))), 0)...
    };
}

template <typename S, typename F, typename T, typename Memfn, size_t... I>
constexpr void foreach_impl(F&& f, T&& t, Memfn&& mfn, std::index_sequence<I...>)
{
    using swallow = int[];
    (void)swallow {
        0, (void(invoke(std::forward<F>(f), std::forward<T>(t).*std::get<I>(mfn), S::mem[I])), 0)...
    };
}

template <typename F, typename T>
constexpr std::enable_if_t<has_series<T>::value> foreach(F&& f, T&& t)
{
    using M = __series<__remove_cvr_t<T>>;
    foreach_impl(std::forward<F>(f), std::forward<T>(t), M::apply(), std::make_index_sequence<M::size>{});
}

template <typename F, typename T>
constexpr std::enable_if_t<!has_series<T>::value> foreach(F&& f, T&& t)
{
    invoke(std::forward<F>(f), std::forward<T>(t));
}

template <typename F, typename T>
constexpr std::enable_if_t<has_series<T>::value> foreach(F&& f, T&& t, const char * const)
{
    using M = __series<__remove_cvr_t<T>>;
    foreach_impl<M>(std::forward<F>(f), std::forward<T>(t), M::apply(), std::make_index_sequence<M::size>{});
}

template <typename F, typename T>
constexpr std::enable_if_t<!has_series<T>::value> foreach(F&& f, T&& t, const char * const name)
{
    invoke(std::forward<F>(f), std::forward<T>(t), name);
}

template <typename T>
constexpr const char * const get_name()
{
    return __series<__remove_cvr_t<T>>::name;
}

}