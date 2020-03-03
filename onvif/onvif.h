//
// Created by Luoxs on 2017-03-16
//
#pragma once

#include <string>
#include <vector>

namespace onvif 
{

struct resolution
{
    int width;
    int height;
};

struct profile
{
    std::string token;
    std::string name;
    std::string video_source_token;
    resolution  encoder_resolution;

    std::string rtsp_uri;
};

struct device
{
    std::string ip;
    std::string username;
    std::string password;

    std::string onvif_uri;
    std::string media_uri;
	std::string event_uri;

    std::vector<profile> profiles;
};

struct event
{
    std::string source_ip;
    std::string topic;
    std::string arrival_time;
    std::string operation;
    std::string source;
    std::string data;
};

std::vector<device> probe_device();

void get_profiles(std::vector<device>& devices);

typedef void (*onvif_event_callback)(const event *);

void device_event_init(std::vector<device>& devices, onvif_event_callback evcb);

} // namespace onvif