//
// Created by Luoxs on 2017-03-16
//
#include "profiles.h"
#include "nsmap.h"

namespace onvif 
{

void get_profiles(std::vector<device>& devices)
{
	for (auto& dev : devices)
		(std::make_shared<attribute>(onvif_io_service, dev))->start();
    onvif_io_service.run();
    onvif_io_service.reset();
}

attribute::attribute(boost::asio::io_service& io_serv, device& dev)
	: m_io_service(io_serv), m_device(dev)
{

}

#define REQUEST_HANDLER(request, handler) {\
    auto self(shared_from_this()); \
    m_connection = std::make_shared<onvif::connection>(m_io_service); \
    m_connection->set_handler(std::bind(&attribute::##request, self), \
        std::bind(&attribute::##handler, self, std::placeholders::_1, std::placeholders::_2)); \
    m_connection->connect(m_device.ip);\
}

void attribute::start()
{
    REQUEST_HANDLER(request_services, handle_services);
}

void attribute::request_services()
{
    auto bodylen = snprintf(m_writebuf, sizeof(m_writebuf), "%s<s:Envelope xmlns:s=\""
        "%s\"><s:Body xmlns:xsi=\"%s\" xmlns:xsd=\"%s\"><GetServices xmlns=\"%s\""
        "><IncludeCapability>false</IncludeCapability></GetServices></s:Body></s:Envelope>",
        XML_PREFACE, WSDL_SOAP_ENVELOPE, WSDL_XSI, WSDL_XS, WSDL_ONVIF_DEVICE);

    auto head = m_writebuf + bodylen;

    auto headlen = snprintf(head, sizeof(m_writebuf) - bodylen, "POST %s HTTP/1.1\r\n"
        "Host: %s\r\nContent-Type: application/soap+xml; charset=utf-8\r\n"
        "Content-Length: %d\r\n\r\n",
        m_device.onvif_uri.c_str(), m_device.ip.c_str(), bodylen);

    m_connection->send(multi_const_buffer(head, headlen, m_writebuf, bodylen));
}

void attribute::handle_services(const char *begin, const char *end)
{
    boost::regex re_media("Namespace>http://www.onvif.org/ver10/media/wsdl<.*?XAddr>http://[0-9\\.]+([/\\w]+)<");
    boost::regex re_event("Namespace>http://www.onvif.org/ver10/events/wsdl<.*?XAddr>http://[0-9\\.]+([/\\w]+)<");
    boost::cmatch m;
    if (boost::regex_search(begin, end, m, re_media))
        m_device.media_uri = m[1].str();

    if (boost::regex_search(begin, end, m, re_event))
        m_device.event_uri = m[1].str();

    if (!m_device.media_uri.empty())
        REQUEST_HANDLER(request_profiles, handle_profiles);
}

void attribute::request_profiles()
{
    auto bodylen = snprintf(m_writebuf, sizeof(m_writebuf), "%s<s:Envelope xmlns:s=\"%s\">",
        XML_PREFACE, WSDL_SOAP_ENVELOPE);

    bodylen += encrypt(m_writebuf + bodylen, m_device.username.c_str(), m_device.password.c_str());

    bodylen += snprintf(m_writebuf + bodylen, 1024, "<s:Body xmlns:xsi=\"%s\" xmlns:xsd=\"%s\">"
        "<GetProfiles xmlns=\"%s\"/></s:Body></s:Envelope>",
        WSDL_XSI, WSDL_XS, WSDL_ONVIF_MEDIA);

    auto head = m_writebuf + bodylen;

    auto headlen = snprintf(head, sizeof(m_writebuf) - bodylen, "POST %s HTTP/1.1\r\n"
        "Host: %s\r\nContent-Type: application/soap+xml; charset=utf-8\r\nContent-Length: %d\r\n\r\n",
        m_device.media_uri.c_str(), m_device.ip.c_str(), bodylen);

    m_connection->send(multi_const_buffer(head, headlen, m_writebuf, bodylen));
}

void attribute::handle_profiles(const char *begin, const char *end)
{
    boost::regex re("Profiles token=\"(\\w+)\".*?Name>(\\w+)<"
        ".*?token=\"(\\w+)\".*?Resolution>.*?"
        "Width>(\\d+)<.*?Height>(\\d+)<");

    auto re_begin = boost::cregex_iterator(begin, end, re);
    auto re_end = boost::cregex_iterator();

    for (auto i = re_begin; i != re_end; ++i)
    {
        auto& m = *i;
        m_device.profiles.push_back(profile());
        auto& pf = m_device.profiles.back();
        pf.token = m[1].str();
        pf.name = m[2].str();
        pf.video_source_token = m[3].str();
        pf.encoder_resolution.width = std::stoi(m[4].str());
        pf.encoder_resolution.height = std::stoi(m[5].str());
    }

    m_profile_index = 0;
    if (m_profile_index < m_device.profiles.size())
        REQUEST_HANDLER(request_streamuri, handle_streamuri);
}

void attribute::request_streamuri()
{
    auto bodylen = snprintf(m_writebuf, sizeof(m_writebuf), "%s<s:Envelope xmlns:s=\"%s\">",
        XML_PREFACE, WSDL_SOAP_ENVELOPE);

    bodylen += encrypt(m_writebuf + bodylen, m_device.username.c_str(), m_device.password.c_str());

    bodylen += snprintf(m_writebuf + bodylen, 1024, "<s:Body xmlns:xsi=\"%s\" xmlns:xsd=\"%s\">"
        "<GetStreamUri xmlns=\"%s\"><StreamSetup><Stream xmlns=\"http://www.onvif.org/ver10/schema"
        "\">RTP-Unicast</Stream><Transport xmlns=\"http://www.onvif.org/ver10/schema"
        "\"><Protocol>UDP</Protocol></Transport></StreamSetup><ProfileToken>%s</ProfileToken>"
        "</GetStreamUri></s:Body></s:Envelope>",
        WSDL_XSI, WSDL_XS, WSDL_ONVIF_MEDIA, m_device.profiles[m_profile_index].token.c_str());

    auto head = m_writebuf + bodylen;

    auto headlen = snprintf(head, sizeof(m_writebuf) - bodylen, "POST %s HTTP/1.1\r\n"
        "Host: %s\r\nContent-Type: application/soap+xml; charset=utf-8\r\nContent-Length: %d\r\n\r\n",
        m_device.media_uri.c_str(), m_device.ip.c_str(), bodylen);

    m_connection->send(multi_const_buffer(head, headlen, m_writebuf, bodylen));
}

void attribute::handle_streamuri(const char *begin, const char *end)
{
    boost::regex re("<tt:Uri>(\\S+?)</tt:Uri>");
    boost::cmatch m;

    if (regex_search(begin, end, m, re))
        m_device.profiles[m_profile_index].rtsp_uri = m[1].str();

    ++m_profile_index;
    if (m_profile_index < m_device.profiles.size())
        REQUEST_HANDLER(request_streamuri, handle_streamuri);
}

}