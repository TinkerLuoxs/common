//
// Created by Luoxs on 2017-03-16
//
#pragma once

#include "onvif_privated.h"

namespace onvif 
{

class event_service
{
public:
    event_service(boost::asio::io_service& io_serv, unsigned short listen_port);

    void async_accept();

private:
    boost::asio::io_service& m_io_service;
    boost::asio::ip::tcp::acceptor m_acceptor;
};


class event_session : public std::enable_shared_from_this<event_session>
{
public:
    event_session(boost::asio::io_service& io_serv);

    void recv_status_line();

    boost::asio::ip::tcp::socket m_socket;

private:

    void recv_headers();

    void recv_content();

    void send_reply();

    boost::asio::streambuf m_readbuf;
    std::string m_reply;
    unsigned int m_content_length;
};

class event_notify : public std::enable_shared_from_this<event_notify>
{
public:
    event_notify(boost::asio::io_service& io_serv, device& dev);

    void start();

    void request_property();
    void request_subscribe();
    void request_renew();
    void request_unsubscribe();


    void handle_property(const char *begin, const char *end);
    void handle_subscribe(const char *begin, const char *end);
    void handle_renew(const char *begin, const char *end);
    void handle_unsubscribe(const char *begin, const char *end);

private:

    device& m_device;
    boost::asio::io_service& m_io_service;
    std::shared_ptr<class connection> m_connection;
    char m_writebuf[2048];
    std::string m_subscribe_index;
};

}