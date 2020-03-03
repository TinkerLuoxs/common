//
// Created by Luoxs on 2017-03-16
//
#include "discovery.h"
#include "nsmap.h"

namespace onvif {

std::vector<device> probe_device()
{
    std::unique_ptr<discovery> ds;
    try
    {
        ds.reset(new discovery(onvif_io_service, get_local_ip()));
        ds->search_network();
    }
    catch (...)
    {
    }
    return std::move(ds->devices);
}

static size_t probe_request_wrap(char *buffer, size_t len)
{
    auto uuid = boost::uuids::to_string(boost::uuids::random_generator()());
    auto size = snprintf(buffer, len, "%s<Envelope xmlns:dn=\"%s\" xmlns=\"%s\""\
        "><Header><wsa:MessageID xmlns:wsa=\"%s\">uuid:%s</wsa:MessageID><wsa:To xmlns:wsa="\
        "\"%s\">%s</wsa:To><wsa:Action xmlns:wsa=\"%s\">%s</wsa:Action></Header><Body>"\
        "<Probe xmlns:xsi=\"%s\" xmlns:xsd=\"%s\" xmlns=\"%s\"><Types>dn:NetworkVideoTransmitter</Types>"\
        "<Scopes /></Probe></Body></Envelope>",
        XML_PREFACE, WSDL_ONVIF_NETWORK, WSDL_SOAP_ENVELOPE, WSDL_WS_ADDRESS,
        uuid.c_str(), WSDL_WS_ADDRESS, URN_DISCOVERY, WSDL_WS_ADDRESS, WSDL_PROBE,
        WSDL_XSI, WSDL_XS, WSDL_DISCOVERY);
    return size;
}

discovery::discovery(boost::asio::io_service& io_service, const std::string& listen_address)
	: m_listen_address(listen_address), m_multicast_socket(io_service), m_timeout_timer(io_service),
	m_multicast_endpoint(boost::asio::ip::address::from_string("239.255.255.250"), 3702),
	m_readbuf(32 * 1024)
{
	boost::asio::ip::udp::endpoint listen_endpoint(
		boost::asio::ip::address::from_string(listen_address), 3702);
	m_multicast_socket.open(listen_endpoint.protocol());
	m_multicast_socket.set_option(boost::asio::ip::udp::socket::reuse_address(true));
	m_multicast_socket.bind(listen_endpoint);

	m_multicast_socket.set_option(boost::asio::ip::multicast::join_group(m_multicast_endpoint.address()));
}

void discovery::search_network()
{
    auto size = probe_request_wrap(m_writebuf, sizeof(m_writebuf));

    m_multicast_socket.send_to(boost::asio::buffer(m_writebuf, size), m_multicast_endpoint);
    receive_response();
}

void discovery::async_search_network()
{
    auto size = probe_request_wrap(m_writebuf, sizeof(m_writebuf));

    m_multicast_socket.async_send_to(boost::asio::buffer(m_writebuf, size), m_multicast_endpoint,
        [this](const boost::system::error_code& ec, size_t)
    {
        if (!ec)
            async_receive_response();
    });
}

void discovery::receive_response()
{
#ifdef _WIN32
    int timeout = 200; // 200ms
#else
    struct timeval timeout = { 0, 200000 }; // 200ms
#endif
    setsockopt(m_multicast_socket.native(), SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));

    do 
    {
        auto size = m_multicast_socket.receive_from(boost::asio::buffer(m_readbuf), m_sender_endpoint);
        if (size == 0)
            break;

        handle_response(size);
    } while (true);
}

void discovery::async_receive_response()
{
    m_multicast_socket.async_receive_from(boost::asio::buffer(m_readbuf), m_sender_endpoint,
        [this](const boost::system::error_code& ec, size_t size)
    {
        if (!ec && m_multicast_socket.is_open())
        {
            boost::system::error_code ignore_ec;
            m_timeout_timer.cancel(ignore_ec);

            handle_response(size);
            async_receive_response();
        }
    });

    m_timeout_timer.expires_from_now(std::chrono::milliseconds(200));
    m_timeout_timer.async_wait([this](const boost::system::error_code& ec)
    {
        if (!ec)
        {
            boost::system::error_code ignore_ec;
            m_multicast_socket.close(ignore_ec);
        }
    });
}

void discovery::handle_response(size_t size)
{
	boost::regex re1("XAddrs>http://([0-9\\.]+)([/\\w]+).*?<");
	boost::cmatch m;

	const char *begin = &m_readbuf[0];
	const char *end = begin + size;
	if (boost::regex_search(begin, end, m, re1))
	{
		auto it = m_ipaddress.insert(m[1].str());
		if (it.second)
		{
			devices.push_back(device());
			auto& back = devices.back();
			back.ip = *(it.first);
			back.onvif_uri = m[2].str();
		}
	}
}

}

