//
// Created by Luoxs on 2016-03-16
// Last modification on 2018-02-02
//
#ifndef __reuse_utility_hpp__
#define __reuse_utility_hpp__

// This is useful when you have to define global unique variables in the header file.
// Usage: static std::vector<int>& g_integers = global_unique_instance;
template <typename T>
struct global_unique
{
    static T instance;
};
template <typename T>
T global_unique<T>::instance;

class global_unique_instance_t
{
public:
    template <typename T> operator T&() const
    {
        return global_unique<T>::instance;
    }
};
global_unique_instance_t const global_unique_instance = {};

namespace reuse {

// Character encoding conversion.
class codeconvert
{
public:
#ifdef _MSC_VER
    constexpr static const unsigned int UTF8 = CP_UTF8;
    constexpr static const unsigned int GBK  = CP_ACP;
#else
    constexpr static const char * const UTF8 = "UTF-8";
    constexpr static const char * const GBK  = "GBK";
#endif

    template <typename FromCode, typename ToCode>
    static std::string convert(std::string const& srcstr, FromCode from, ToCode to)
    {
#ifdef _MSC_VER
        auto inbytes = MultiByteToWideChar(from, 0, srcstr.data(), -1, NULL, 0);
        std::vector<wchar_t> winput(inbytes + 1, 0);
        MultiByteToWideChar(from, 0, srcstr.data(), -1, winput.data(), inbytes);
        auto outbytes = WideCharToMultiByte(to, 0, winput.data(), -1, NULL, 0, NULL, NULL);
        std::vector<char> output(outbytes + 1, 0);
        WideCharToMultiByte(to, 0, winput.data(), -1, output.data(), outbytes, NULL, NULL);
        return output.data();
#else
        iconv_t convfd = iconv_open(to, from);
        if (convfd == (iconv_t)-1)
            return srcstr;
        auto inbytes = srcstr.size();
        auto outbytes = inbytes * sizeof(wchar_t);
        std::vector<char> output(outbytes + 1, 0);
        auto input_ptr = const_cast<char *>(srcstr.data());
        auto output_ptr = output.data();
        iconv(convfd, &input_ptr, &inbytes, &output_ptr, &outbytes);
        iconv_close(convfd);
        return output.data();
#endif
    }
};

// Localization functions
class localize
{
public:
    static std::string current_timestamp()
    {
        char szCurrentDateTime[32] = { 0 };
        auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::strftime(szCurrentDateTime, sizeof(szCurrentDateTime), "%F %T", std::localtime(&t));
        return szCurrentDateTime;
    }

    static std::vector<std::string> ipconfig()
    {
        boost::system::error_code ignore_code;
        auto hostname = boost::asio::ip::host_name(ignore_code);

        boost::asio::detail::addrinfo_type hints = {};
        boost::asio::detail::addrinfo_type *result = nullptr;
        hints.ai_family = AF_INET;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_socktype = SOCK_STREAM;

        boost::asio::detail::socket_ops::getaddrinfo(hostname.c_str(), nullptr,
            hints, &result, ignore_code);

        std::vector<std::string> iplist;
        for (auto cur = result; cur != nullptr; cur = cur->ai_next)
        {
            char addr_str[boost::asio::detail::max_addr_v4_str_len] = { 0 };
            boost::asio::detail::socket_ops::inet_ntop(BOOST_ASIO_OS_DEF(AF_INET),
                &((struct sockaddr_in *)cur->ai_addr)->sin_addr, addr_str,
                boost::asio::detail::max_addr_v4_str_len, 0, ignore_code);

            iplist.push_back(addr_str);
        }

        boost::asio::detail::socket_ops::freeaddrinfo(result);
        return std::move(iplist);
    }

};

} //namespace reuse

#endif // __reuse_utility_hpp__
