//
// Created by Luoxs on 2017-03-16
//
#include "event.h"
#include "nsmap.h"

namespace onvif 
{

static onvif_event_callback event_callback;
static const unsigned short event_listen_port = 8088;
static const std::string event_terminationtime = "2038-01-01T00:00:00Z";
static std::string event_service_address = "http://";

void device_event_init(std::vector<device>& devices, onvif_event_callback evcb)
{
    event_callback = evcb;

    event_service ev_serv(onvif_io_service, event_listen_port);
    ev_serv.async_accept();

    event_service_address += get_local_ip();
    event_service_address += ":";
    event_service_address += std::to_string(event_listen_port);
    event_service_address += "/onvif_notify_service";


    for (auto& dev : devices)
        (std::make_shared<event_notify>(onvif_io_service, dev))->start();

    onvif_io_service.run();
    onvif_io_service.reset();
}


event_service::event_service(boost::asio::io_service& io_serv, unsigned short listen_port)
    : m_io_service(io_serv), m_acceptor(io_serv, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), listen_port))
{

}

void event_service::async_accept()
{
    auto new_connection = std::make_shared<event_session>(m_io_service);

    m_acceptor.async_accept(new_connection->m_socket,
            [this, new_connection](const boost::system::error_code& ec)
    {
        if (!ec)
            new_connection->recv_status_line();
        async_accept();
    });
}


event_notify::event_notify(boost::asio::io_service& io_serv, device& dev)
    : m_io_service(io_serv), m_device(dev)
{

}

#define REQUEST_HANDLER(request, handler) {\
    auto self(shared_from_this()); \
    m_connection = std::make_shared<connection>(m_io_service); \
    m_connection->set_handler(std::bind(&event_notify::request, self), \
        std::bind(&event_notify::handler, self, std::placeholders::_1, std::placeholders::_2)); \
    m_connection->connect(m_device.ip);\
}

void event_notify::start()
{
    REQUEST_HANDLER(request_property, handle_property);
}

void event_notify::request_property()
{
    auto bodylen = snprintf(m_writebuf, sizeof(m_writebuf), "%s<s:Envelope xmlns:s=\"%s\">",
        XML_PREFACE, WSDL_SOAP_ENVELOPE);

    bodylen += encrypt(m_writebuf + bodylen, m_device.username.c_str(), m_device.password.c_str());

    bodylen += snprintf(m_writebuf + bodylen, 1024, "<s:Body xmlns:xsi=\"%s\" xmlns:xsd=\"%s\">"
        "<GetEventProperties xmlns=\"%s\"/></s:Body></s:Envelope>",
        WSDL_XSI, WSDL_XS, WSDL_ONVIF_EVENT);

    auto head = m_writebuf + bodylen;

    auto headlen = snprintf(head, sizeof(m_writebuf) - bodylen, "POST %s HTTP/1.1\r\n"
        "Host: %s\r\nContent-Type: application/soap+xml; charset=utf-8\r\nContent-Length: %d\r\n\r\n",
        m_device.event_uri.c_str(), m_device.ip.c_str(), bodylen);

    m_connection->send(multi_const_buffer(head, headlen, m_writebuf, bodylen));
}

void event_notify::request_subscribe()
{
    auto bodylen = snprintf(m_writebuf, sizeof(m_writebuf), "%s<s:Envelope xmlns:s=\"%s\" xmlns:a="
        "\"http://www.w3.org/2005/08/addressing\">",
        XML_PREFACE, WSDL_SOAP_ENVELOPE);

    bodylen += encrypt(m_writebuf + bodylen, m_device.username.c_str(), m_device.password.c_str());

    bodylen += snprintf(m_writebuf + bodylen, 1024, "<s:Body xmlns:xsi=\"%s\" xmlns:xsd=\"%s\">"
        "<Subscribe xmlns=\"http://docs.oasis-open.org/wsn/b-2\"><ConsumerReference><a:Address>"
        "%s</a:Address></ConsumerReference><InitialTerminationTime>%s</InitialTerminationTime>"
        "</Subscribe></s:Body></s:Envelope>",
        WSDL_XSI, WSDL_XS, event_service_address.c_str(), event_terminationtime.c_str());

    auto head = m_writebuf + bodylen;

    auto headlen = snprintf(head, sizeof(m_writebuf) - bodylen, "POST %s HTTP/1.1\r\n"
        "Host: %s\r\nContent-Type: application/soap+xml; charset=utf-8\r\nContent-Length: %d\r\n\r\n",
        m_device.event_uri.c_str(), m_device.ip.c_str(), bodylen);

    m_connection->send(multi_const_buffer(head, headlen, m_writebuf, bodylen));
}

void event_notify::request_renew()
{
    auto bodylen = snprintf(m_writebuf, sizeof(m_writebuf), "%s<s:Envelope xmlns:s=\"%s\">",
        XML_PREFACE, WSDL_SOAP_ENVELOPE);

    bodylen += encrypt(m_writebuf + bodylen, m_device.username.c_str(), m_device.password.c_str());

    bodylen += snprintf(m_writebuf + bodylen, 1024, "<s:Body xmlns:xsi=\"%s\" xmlns:xsd=\"%s\">"
        "<Renew xmlns=\"http://docs.oasis-open.org/wsn/b-2\"><TerminationTime>%s</TerminationTime>"
        "</Renew></s:Body></s:Envelope>",
        WSDL_XSI, WSDL_XS, event_terminationtime.c_str());

    auto head = m_writebuf + bodylen;

    auto headlen = snprintf(head, sizeof(m_writebuf) - bodylen, "POST %s HTTP/1.1\r\n"
        "Host: %s\r\nContent-Type: application/soap+xml; charset=utf-8\r\nContent-Length: %d\r\n\r\n",
        m_subscribe_index.c_str(), m_device.ip.c_str(), bodylen);

    m_connection->send(multi_const_buffer(head, headlen, m_writebuf, bodylen));
}

void event_notify::request_unsubscribe()
{
    auto bodylen = snprintf(m_writebuf, sizeof(m_writebuf), "%s<s:Envelope xmlns:s=\"%s\">",
        XML_PREFACE, WSDL_SOAP_ENVELOPE);

    bodylen += encrypt(m_writebuf + bodylen, m_device.username.c_str(), m_device.password.c_str());

    bodylen += snprintf(m_writebuf + bodylen, 1024, "<s:Body xmlns:xsi=\"%s\" xmlns:xsd=\"%s\">"
        "<Unsubscribe xmlns=\"http://docs.oasis-open.org/wsn/b-2\"/></s:Body></s:Envelope>",
        WSDL_XSI, WSDL_XS);

    auto head = m_writebuf + bodylen;

    auto headlen = snprintf(head, sizeof(m_writebuf) - bodylen, "POST %s HTTP/1.1\r\n"
        "Host: %s\r\nContent-Type: application/soap+xml; charset=utf-8\r\nContent-Length: %d\r\n\r\n",
        m_subscribe_index.c_str(), m_device.ip.c_str(), bodylen);

    m_connection->send(multi_const_buffer(head, headlen, m_writebuf, bodylen));
}

void event_notify::handle_property(const char *begin, const char *end)
{
    //printf("[event properties] %s\n", begin);

    REQUEST_HANDLER(request_subscribe, handle_subscribe);
}

void event_notify::handle_subscribe(const char *begin, const char *end)
{
    //printf("[event subscribe] %s\n", begin);
    boost::regex re("Address>http://.+?(/.+?)<");
    boost::cmatch m;
    if (boost::regex_search(begin, end, m, re))
        m_subscribe_index = m[1].str();
}

void event_notify::handle_renew(const char *begin, const char *)
{
    printf("[event renew subscribe] %s\n", begin);
}

void event_notify::handle_unsubscribe(const char *begin, const char *)
{
    printf("[event unsubscribe] %s\n", begin);
}

event_session::event_session(boost::asio::io_service& io_serv)
    : m_socket(io_serv)
{
    m_reply = "HTTP/1.1 200 OK\r\n";
    m_reply += "Content-Length: 0\r\n";
    m_reply += "Connection: close\r\n";
    m_reply += "\r\n";
}

void event_session::recv_status_line()
{
    auto self(shared_from_this());
    boost::asio::async_read_until(m_socket, m_readbuf, "\r\n",
        [this, self](const boost::system::error_code& ec, size_t)
    {
        if (ec)
            return;

        std::istream response_stream(&m_readbuf);
        std::string http_method, uri, http_version;
        response_stream >> http_method >> uri >> http_version;

        std::getline(response_stream, http_method);
        if (!response_stream || http_version.compare(0, 5, "HTTP/"))
        {
            printf("[onvif_notify_service:%hu] receive invalid response\n", event_listen_port);
            return;
        }

        recv_headers();
    });
}

void event_session::recv_headers()
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

void event_session::recv_content()
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

        boost::regex re("Topic.*?>(.+?)<.*?UtcTime=\"(.+?)\".*PropertyOperation=\"(.+?)\""
                        ".*?Source>(.+?)Source>.*?Data>(.+?)Data>");
        boost::cmatch m;
        if (boost::regex_search(buf, m, re))
        {
            event ev;
            ev.topic = m[1].str();
            ev.arrival_time = m[2].str();
            ev.operation = m[3].str();

            boost::regex re_("SimpleItem Name=\"(.+?)\" Value=\"(.+?)\"");

            auto re_begin = boost::cregex_iterator(m[4].begin(), m[4].end(), re_);
            auto re2_begin = boost::cregex_iterator(m[5].begin(), m[5].end(), re_);
            auto re_end = boost::cregex_iterator();
            for (auto i = re_begin; i != re_end; ++i)
            {
                auto& m = *i;
                if (!ev.source.empty())
                    ev.source += "; ";
                ev.source += m[1].str();
                ev.source += ": ";
                ev.source += m[2].str();   
            }

            for (auto i = re2_begin; i != re_end; ++i)
            {
                auto& m = *i;
                if (!ev.data.empty())
                    ev.data += "; ";
                ev.data += m[1].str();
                ev.data += ": ";
                ev.data += m[2].str();
            }

            auto remote_endpt = m_socket.remote_endpoint();
            ev.source_ip = remote_endpt.address().to_string();
            event_callback(&ev);
        }

        delete[] buf;

        send_reply();
    });
}

void event_session::send_reply()
{
    auto self(shared_from_this());
    boost::asio::async_write(m_socket, boost::asio::buffer(m_reply),
        [this, self](const boost::system::error_code& ec, size_t)
    {
    });
}

}