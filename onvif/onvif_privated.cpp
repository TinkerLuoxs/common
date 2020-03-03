//
// Created by Luoxs on 2017-03-16
//
#include "onvif_privated.h"
#include "encryption.h"
#ifdef _WIN32
#pragma warning(disable:4996)
#endif
namespace onvif
{

boost::asio::io_service onvif_io_service;

std::string get_local_ip()
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

    if (result == nullptr)
        throw ignore_code.message();

    char ip[32] = {0};
    boost::asio::detail::socket_ops::inet_ntop(
        BOOST_ASIO_OS_DEF(AF_INET), &((struct sockaddr_in *)result->ai_addr)->sin_addr,
        ip, sizeof(ip), 0, ignore_code);

    boost::asio::detail::socket_ops::freeaddrinfo(result);
    return ip;
}

int encrypt(char *outbuf, const char *usr, const char *pwd)
{
    char created[32] = {};
    char nonce[24] = {};
    time_t t = time(NULL);

    //generate created.
    int created_len = (int)strftime(created, sizeof(created), "%Y-%m-%dT%H:%M:%S", localtime(&t));
    if (!created_len)
        created_len = snprintf(created, sizeof(created), "1969-12-31T23:59:59Z");

    //generate nonce.
    static short nonce_count = (short)0xCA53;
    /* we could have used raw binary instead of hex as below */
    int nonce_len = snprintf(nonce, 20, "%8.8x%4.4hx%8.8x", (int)t, nonce_count++, std::rand());

    //encode nonce.
    auto nonce_base64 = base64::encode(nonce, nonce_len);

    //generate password digest
    sha::sha1 sha1;
    char sha1_hash[24] = {};
    sha1.append(nonce, nonce_len);
    sha1.append(created, created_len);
    sha1.append(pwd, strlen(pwd));
    sha1.finish((unsigned char *)sha1_hash);
    auto pwd_digest = base64::encode(sha1_hash, 20);

    //encode security
    int size = snprintf(outbuf, 1024, "<s:Header><Security s:mustUnderstand=\"1\" xmlns=\""
        "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd"
        "\"><UsernameToken><Username>%s</Username><Password Type=\""
        "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordDigest"
        "\">%s</Password><Nonce EncodingType=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-soap-message-security-1.0#Base64Binary"
        "\">%s</Nonce><Created xmlns=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd"
        "\">%s</Created></UsernameToken></Security></s:Header>",
        usr, pwd_digest.c_str(), nonce_base64.c_str(), created);

    return size;
}

connection::connection(boost::asio::io_service& io_serv)
    : m_socket(io_serv)
{

}

void connection::set_handler(event_handler&& on_connect, response_handler&& on_read)
{
    m_handle_connect = std::move(on_connect);
    m_handle_response = std::move(on_read);
}

void connection::connect(const std::string& ip)
{
    auto self(shared_from_this());
    boost::asio::ip::tcp::endpoint endpt(boost::asio::ip::address::from_string(ip), 80);
    m_socket.async_connect(endpt, [this, self](const boost::system::error_code& ec)
    {
        if (!ec)
            m_handle_connect();
    });
}

void connection::send(multi_const_buffer wbuf)
{
    auto self(shared_from_this());
    boost::asio::async_write(m_socket, wbuf,
        [this, self](const boost::system::error_code& ec, size_t)
    {
        if (!ec)
            recv_status_line();
    });
}

void connection::recv_status_line()
{
    auto self(shared_from_this());
    boost::asio::async_read_until(m_socket, m_readbuf, "\r\n",
        [this, self](const boost::system::error_code& ec, size_t)
    {
        if (ec)
            return;

        std::istream response_stream(&m_readbuf);
        std::string http_version;
        response_stream >> http_version;
        unsigned int status_code;
        response_stream >> status_code;
        std::string status_message;
        std::getline(response_stream, status_message);
        if (!response_stream || http_version.substr(0, 5) != "HTTP/")
        {
            printf("Invalid response\n");
            return;
        }
        if (status_code != 200)
        {
            printf("Response returned with status code %u\n", status_code);
            return;
        }

        recv_headers();
    });
}

void connection::recv_headers()
{
    auto self(shared_from_this());
    boost::asio::async_read_until(m_socket, m_readbuf, "\r\n\r\n",
        [this, self](const boost::system::error_code& ec, size_t)
    {
        if (ec)
            return;

        std::istream response_stream(&m_readbuf);
        std::string header;
        m_content_length = 0;
        while (std::getline(response_stream, header) && header != "\r")
        {
            if (!header.compare(0, 16, "Content-Length: "))
                m_content_length = std::stoi(header.substr(16));
        }

        recv_content();
    });
}

void connection::recv_content()
{
    auto self(shared_from_this());
    boost::asio::async_read(m_socket, m_readbuf,
        boost::asio::transfer_exactly(m_content_length - m_readbuf.size()),
        [this, self](const boost::system::error_code& ec, size_t)
    {
        if (ec)
            return;

        auto size = m_readbuf.size();
        std::istream content_stream(&m_readbuf);
        char *buf = new char[size + 1];
        content_stream.read(buf, size);
        m_handle_response(buf, buf + size);
        delete[] buf;
    });
}

}