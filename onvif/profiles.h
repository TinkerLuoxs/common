//
// Created by Luoxs on 2017-03-16
//
#pragma once

#include "onvif_privated.h"

namespace onvif 
{

class attribute : public std::enable_shared_from_this<attribute>
{
public:
	attribute(boost::asio::io_service& io_serv, device& dev);

	void start();

	void request_services();
	void request_profiles();
	void request_streamuri();

	void handle_services(const char *begin, const char *end);
	void handle_profiles(const char *begin, const char *end);
	void handle_streamuri(const char *begin, const char *end);

private:

	device& m_device;
	boost::asio::io_service& m_io_service;
	std::shared_ptr<class connection> m_connection;
    char m_writebuf[2048];
    unsigned int m_profile_index;
};

}