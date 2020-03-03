//
// Created by Luoxs on 2017-03-16
//
#pragma once

#include <boost/regex.hpp>
#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/random_generator.hpp>

namespace onvif
{
    extern boost::asio::io_service onvif_io_service;

    std::string get_local_ip();

    int encrypt(char *outbuf, const char *usr, const char *pwd);

    class multi_const_buffer
    {
    public:
        explicit multi_const_buffer(const void *data1, size_t size1, const void *data2, size_t size2)
        {
            buffer_[0] = boost::asio::buffer(data1, size1);
            buffer_[1] = boost::asio::buffer(data2, size2);
        }

        typedef boost::asio::const_buffer value_type;
        typedef const boost::asio::const_buffer* const_iterator;
        const boost::asio::const_buffer* begin() const { return buffer_; }
        const boost::asio::const_buffer* end() const { return buffer_ + 2; }

    private:
        boost::asio::const_buffer buffer_[2];
    };

    class connection : public std::enable_shared_from_this<connection>
    {
    public:
        typedef std::function<void()> event_handler;
        typedef std::function<void(char *begin, char *end)> response_handler;
        connection(boost::asio::io_service& io_serv);

        void set_handler(event_handler&& on_connect, response_handler&& on_read);

        void connect(const std::string& ip);

        void send(multi_const_buffer wbuf);

    private:

        void recv_status_line();

        void recv_headers();

        void recv_content();

        boost::asio::ip::tcp::socket m_socket;
        boost::asio::streambuf m_readbuf;
        event_handler m_handle_connect;
        response_handler m_handle_response;
        int m_content_length;
    };
}

#include "onvif.h"