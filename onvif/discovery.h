//
// Created by Luoxs on 2017-03-16
//
#pragma once

#include <set>
#include "onvif_privated.h"

namespace onvif 
{

class discovery
{
public:
    discovery(boost::asio::io_service& io_service, const std::string& listen_address);

    void search_network();

    void async_search_network();

	std::vector<device> devices;

private:
    void receive_response();

    void async_receive_response();

    void handle_response(size_t size);

	const std::string& m_listen_address;
    boost::asio::ip::udp::socket m_multicast_socket;
    boost::asio::ip::udp::endpoint m_multicast_endpoint;
    boost::asio::ip::udp::endpoint m_sender_endpoint;
    boost::asio::steady_timer m_timeout_timer;
	std::vector<char> m_readbuf;
    char m_writebuf[1024];
	std::set<std::string> m_ipaddress;
};

}