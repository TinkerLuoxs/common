//
// Created by Luoxs on 2017-04-19
//
#pragma once

#include "jt808_type.h"

//////////////////////////////////////////////////////////////////////////
//message id	
#define MSG808_DEV_COMMON_RSP				0x0001      
#define MSG808_SERV_COMMON_RSP				0x8001      
#define MSG808_DEV_HEARTBEAT				0x0002      
#define MSG808_DEV_REGISTER_REQ				0x0100      
#define MSG808_DEV_REGISTER_RSP				0x8100      
#define MSG808_DEV_UNREGISTER				0x0003      
#define MSG808_DEV_AUTHORISE				0x0102      
#define MSG808_SET_DEV_PARAM_REQ			0x8103      
#define MSG808_QUERY_DEV_ALL_PARAM_REQ		0x8104  
#define MSG808_QUERY_DEV_PART_PARAM_REQ		0x8106
#define MSG808_QUERY_DEV_PARAM_RSP			0x0104      
#define MSG808_QUERY_DEV_PROPERTY_REQ		0x8107      
#define MSG808_QUERY_DEV_PROPERTY_RSP		0x0107      
#define MSG808_DEV_GPS_REPORT				0x0200      
#define MSG808_QUERY_DEV_GPS_REQ			0x8201      
#define MSG808_QUERY_DEV_GPS_RSP			0x0201      
#define MSG808_QUERY_DRIVER_RSP				0x0702      
#define MSG808_QUERY_DRIVER_REQ				0x8702 
#define MSG808_NOTIFY_DEV				    0x8300
#define MSG808_PHONE_CALLBACK				0x8400
#define MSG808_SET_RECORD					0x8804
#define MSG808_DOWN_TUNNEL_DATA				0x8900      
#define MSG808_UP_TUNNEL_DATA				0x0900   
#define MSG808_DEV_CONTROL_REQ              0x8105
#define MSG808_DEV_UPGRADE_REQ              0x8108
#define MSG808_DEV_UPGRADE_RSP              0x0108
#define MSG808_DEV_TRACE_REQ                0x8202
#define MSG808_VERIFY_ALARM                 0x8203

//////////////////////////////////////////////////////////////////////////
//device parameter id
#define PARAM_HEARTBEAT					0x0001      //value   units: sec
#define PARAM_TCP_TIMEOUT				0x0002      //value   units: sec
#define PARAM_TCP_RETRY					0x0003      //value              
#define PARAM_UDP_TIMEOUT				0x0004      //value   units: sec
#define PARAM_UDP_RETRY					0x0005      //value 
#define PARAM_SMS_TIMEOUT				0x0006      //value   units: sec
#define PARAM_SMS_RETRY					0x0007      //value 
/*    reserve                0x0008 - 0x000F             */
#define PARAM_HOST_DIAL_ACCESS			0x0010      //value      
#define PARAM_HOST_DIAL_USERNAME		0x0011      //value  
#define PARAM_HOST_DIAL_PASSWORD		0x0012      //value  
#define PARAM_HOST_ADDR					0x0013      //value  
#define PARAM_BACKUP_DIAL_ACCESS		0x0014      //value  
#define PARAM_BACKUP_DIAL_USERNAME		0x0015      //value  
#define PARAM_BACKUP_DIAL_PASSWORD		0x0016      //value  
#define PARAM_BACKUP_ADDR				0x0017      //value  
#define PARAM_TCP_PORT					0x0018      //value 
#define PARAM_UDP_PORT					0x0019      //value 
#define PARAM_AUTH_HOST_ADDR			0x001A      //value          
#define PARAM_AUTH_HOST_TCP_PORT		0x001B      //value 
#define PARAM_AUTH_HOST_UDP_PORT		0x001C      //value 
#define PARAM_AUTH_BACKUP_TCP_ADDR		0x001D      //value  
/*    reserve               0x001E  - 0x001F             */
#define PARAM_POSITION_STRATEGY			0x0020      //value   0:timing,1:interval,2:timing and interval
#define PARAM_POSITION_SCHEME			0x0021      //value   0:ACC,1:login and ACC
#define PARAM_DRIVER_NOLOGIN_TIME		0x0022      //value   units: sec
/*    reserve               0x0023  - 0x0026             */
#define PARAM_DORMANT_TIME				0x0027      //value   units: sec
#define PARAM_URGENT_ALARM_TIME			0x0028      //value   units: sec
#define PARAM_DEFAULT_REPORT_TIME		0x0029      //value   units: sec
/*    reserve               0x002A  - 0x002B             */
#define PARAM_DEFAULT_DISTANCE			0x002C      //value   units: m
#define PARAM_DRIVER_NOLOGIN_DISTANCE   0x002D      //value   units: m
#define PARAM_DORMANT_DISTANCE			0x002E      //value   units: m
#define PARAM_URGENT_ALARM_DISTANCE		0x002F      //value   units: m
#define PARAM_CURVE_ANGLE				0x0030      //value   < 180 angle
#define PARAM_FENCE_RADIUS				0x0031      //value   units: m
/*    reserve               0x0032  - 0x003F             */
#define PARAM_MONITOR_SERV_TEL          0x0040      //value  
#define PARAM_RESET_TEL					0x0041      //value  
#define PARAM_DEFAULT_TEL				0x0042      //value  
#define PARAM_MONITOR_SERV_SMS_TEL      0x0043      //value  
#define PARAM_DEV_SMS_ALARM_TEL         0x0044      //value  
#define PARAM_DEV_ANSWER_TEL_STRATEGY   0x0045      //value   0:auto,1:auto when ACC on,or else manual
#define PARAM_ONCE_MAX_CALL_TIME		0x0046      //value   units: sec,0:not allow,0xFFFFFFFF:not limit
#define PARAM_MONTH_MAX_CALL_TIME	    0x0047      //value   units: sec,0:not allow,0xFFFFFFFF:not limit
#define PARAM_LISTEN_TEL				0x0048      //value  
#define PARAM_PRIVILEGE_SMS_TEL         0x0049      //value  
/*    reserve               0x004A  - 0x004F             */
#define PARAM_ALARM_HIDE				0x0050      //value   
#define PARAM_ALARM_SMS_SWITCH			0x0051      //value 
#define PARAM_ALARM_PHOTO_SWITCH		0x0052      //value 
#define PARAM_ALARM_PHOTO_STORE			0x0053      //value 
#define PARAM_MAJOR_ALARM				0x0054      //value 
#define PARAM_HIGHEST_SPEED				0x0055      //value   units: km/h
#define PARAM_OVERSPEED_TIME			0x0056      //value   units: sec
#define PARAM_ONCE_DRIVE_THRESHOLD		0x0057      //value   units: sec
#define PARAM_DAY_DRIVE_THRESHOLD		0x0058      //value   units: sec
#define PARAM_MIN_REST_TIME				0x0059      //value   units: sec
#define PARAM_MAX_PARK_TIME				0x005A      //value   units: sec
#define PARAM_OVERSPEED_DEVIATION		0x005B      //value   units: 1/10km/h
#define PARAM_FATIGUEDRIVE_DEVIATION	0x005C      //value   units: sec
#define PARAM_COLLIDE_ALARM				0x005D      //value   b7-b0:units: sec, b15-b8:units: 0.1g 0-79
#define PARAM_ROLLOVER_ALARM			0x005E      //value   units: angle,default 30 angle
/*    reserve               0x005F  - 0x0063             */
#define PARAM_PHOTO_TIMING				0x0064      //value 
#define PARAM_PHOTO_DISTANCE			0x0065      //value 
/*    reserve               0x0066  - 0x006F             */
#define PARAM_IMAGE_QUALITY				0x0070      //value   1-10,1 is the best
#define PARAM_IMAGE_BRIGHTNESS			0x0071      //value   0-255
#define PARAM_IMAGE_CONTRAST			0x0072      //value   0-127
#define PARAM_IMAGE_SATURATION			0x0073      //value   0-127
#define PARAM_IMAGE_CHROMA				0x0074      //value   0-255
/*    reserve               0x0075  - 0x007F             */
#define PARAM_MILEAGE					0x0080      //value   units: 1/10km
#define PARAM_PROVINCE_ID				0x0081      //value         
#define PARAM_CITY_ID					0x0082      //value     
#define PARAM_CARPLATE					0x0083      //value              
#define PARAM_CARPALTE_COLOR			0x0084      //value                    
#define PARAM_GNSS_POSITION				0x0090      //value 
#define PARAM_GNSS_BAUDRATE				0x0091      //value 
#define PARAM_GNSS_OUTPUT_FREQUENCY		0x0092      //value 
#define PARAM_GNSS_SAMPLE_FREQUENCY		0x0093      //value 
#define PARAM_GNSS_UPLOAD_MODE			0x0094      //value 
#define PARAM_GNSS_UPLOAD_SETUP			0x0095      //value 
#define PARAM_CAN1_SAMPLE_TIME			0x0100      //value 
#define PARAM_CAN1_UPLOAD_TIME			0x0101      //value 
#define PARAM_CAN2_SAMPLE_TIME			0x0102      //value 
#define PARAM_CAN2_UPLOAD_TIME			0x0103      //value  
#define PARAM_CAN_ALONE_SAMPLE_SETUP    0x0110      //value         
/*  other CAN ID reserve    0x0111  - 0x01FF             */
/*  user-defined reserve    0x001E  - 0x001F             */


//////////////////////////////////////////////////////////////////////////
//alarm type 
#define ALARM_URGENCY               0x0000000000000001ULL
#define ALARM_OVERSPEED             0x0000000000000002ULL
#define ALARM_FATIGUE_DRIVE         0x0000000000000004ULL  
#define ALARM_DANGER_WARNING        0x0000000000000008ULL
#define ALARM_GNSS_FAULT            0x0000000000000010ULL
#define ALARM_GNSS_NOCONNECT        0x0000000000000020ULL
#define ALARM_GNSS_SHORT_CIRCUIT    0x0000000000000040ULL
#define ALARM_LESS_VOLTAGE			0x0000000000000080ULL
#define ALARM_ELEC_LEAKAGE          0x0000000000000100ULL
#define ALARM_LCD_FAULT             0x0000000000000200ULL
#define ALARM_TTS_MODULE_FAULT      0x0000000000000400ULL
#define ALARM_CAMERA_FAULT          0x0000000000000800ULL
#define ALARM_ICCARD_MODULE_FAULT   0x0000000000001000ULL
#define ALARM_OVERSPEED_WARNING     0x0000000000002000ULL
#define ALARM_FATIGUE_DRIVE_WARNING 0x0000000000004000ULL
#define ALARM_ELOCK_LOST			0x0000000000008000ULL
/*   bit 16 - 17    jt808 reserve                      */
#define ALARM_DRIVE_TIMEOUT			0x0000000000040000ULL
#define ALARM_PARK_TIMEOUT          0x0000000000080000ULL
#define ALARM_AREA					0x0000000000100000ULL
#define ALARM_ROUTE					0x0000000000200000ULL
#define ALARM_DRIVE_TIME			0x0000000000400000ULL
#define ALARM_ROUTE_DEVIATE         0x0000000000800000ULL
#define ALARM_VSS_FAULT             0x0000000001000000ULL
#define ALARM_OILMASS_UNUSUAL		0x0000000002000000ULL
#define ALARM_VEHICLE_STOLEN        0x0000000004000000ULL
#define ALARM_ILLEGALITY_IGNITION   0x0000000008000000ULL
#define ALARM_ILLEGALITY_DISPLACE   0x0000000010000000ULL
#define ALARM_COLLISION_WARNING     0x0000000020000000ULL
#define ALARM_ROLLOVER_WARNING      0x0000000040000000ULL
#define ALARM_ILLEGALITY_OPENDOOR   0x0000000080000000ULL
/*   bit 32 - 62 user-defined extension                */

//////////////////////////////////////////////////////////////////////////
//GPS status value
#define GPS_STATUS_ACC_ON				0x00000001U
#define GPS_STATUS_LOCATION_ON			0x00000002U
#define GPS_STATUS_SOUTH_LATITUDE		0x00000004U
#define GPS_STATUS_WEST_LONGITUDE		0x00000008U
#define GPS_STATUS_STOP_TRANSPORT		0x00000010U
#define GPS_STATUS_ENCRYPT_SITE			0x00000020U
#define GPS_STATUS_RESERVE6				0x00000040U
#define GPS_STATUS_RESERVE7				0x00000080U
#define GPS_STATUS_ENOUGH				0x00000100U //combine GPS_STATUS_VEHICLE, bit8bit9, 00:empty of the car,
#define GPS_STATUS_VEHICLE				0x00000200U //01:half of the car, 10: reserve, 11: enough of the cars
#define GPS_STATUS_OIL_DISCONECT		0x00000400U
#define GPS_STATUS_ELEC_DISCONNECT		0x00000800U
#define GPS_STATUS_DOOR_ONLOCK			0x00001000U
#define GPS_STATUS_DOOR1_OPEN			0x00002000U
#define GPS_STATUS_DOOR2_OPEN			0x00004000U
#define GPS_STATUS_DOOR3_OPEN			0x00008000U
#define GPS_STATUS_DOOR4_OPEN			0x00010000U
#define GPS_STATUS_DOOR5_OPEN			0x00020000U
#define GPS_STATUS_GPS_ON				0x00040000U
#define GPS_STATUS_BEIDOU_ON			0x00080000U
#define GPS_STATUS_GLONASS_ON			0x00100000U
#define GPS_STATUS_GALILEO_ON			0x00200000U

//////////////////////////////////////////////////////////////////////////
//GPS annex message id
#define	GPS_ANNEX_MILEAGE            0x01		
#define	GPS_ANNEX_OIL_VOLUME         0x02	
#define	GPS_ANNEX_RECORD_SPEED       0x03	
#define	GPS_ANNEX_ALARM_EVENTID      0x04
/*  0x05 - 0x10  jt808 reserve         */
#define	GPS_ANNEX_OVERSPEED          0x11
#define	GPS_ANNEX_AREA_ALRAM         0x12		
#define	GPS_ANNEX_TRAVEL_TIME        0x13	
/*  0x14 - 0x24  jt808 reserve         */
#define	GPS_ANNEX_CAR_SIGNAL_STATUS  0x25	
#define	GPS_ANNEX_IO_STATUS          0x2A
#define	GPS_ANNEX_ANALOG_QUANTITY    0x2B
#define	GPS_ANNEX_WIRELESS_SIGNAL    0x30
#define	GPS_ANNEX_GNSS_SATELLITE     0x31
/*  0xE1 - 0xFF  user-defined reserve  */

//////////////////////////////////////////////////////////////////////////
//notify type
#define	NOTIFY_URGENCY	    0x01		
#define	NOTIFY_DISPLAY	    0x04	
#define	NOTIFY_TTS		    0x08	
#define	NOTIFY_ADVERT		0x10
#define	NOTIFY_CANBUG		0x20

//////////////////////////////////////////////////////////////////////////
//message structure

#define DECLARE_SERIALIZE() template <typename Archive> void serialize(Archive& ar)

#define DECLARE_MESSAGE(id) constexpr static const t_uint16 msgID = id; DECLARE_SERIALIZE()

namespace jt 
{

struct head808
{
	t_uint16 msgID;
	t_uint16 msgSeq;
    t_string phone;
};

struct dev_common_reply
{
	t_uint16 reqSeq;
	t_uint16 reqMsgID;
	t_uint8	 result;		//0-OK, 1-failure, 2-msg error, 3-nonsupport

	DECLARE_MESSAGE(MSG808_DEV_COMMON_RSP);
};

struct serv_common_reply
{
    t_uint16 reqSeq;
    t_uint16 reqMsgID;
	t_uint8	 result;		//0-OK, 1-failure, 2-msg error, 3-nonsupport, 4-alarm verify

 	DECLARE_MESSAGE(MSG808_SERV_COMMON_RSP);
};

struct heartbeat_req
{
 	DECLARE_MESSAGE(MSG808_DEV_HEARTBEAT);
};

struct register_req
{
    t_uint16    provinceID;
    t_uint16    cityID;
    t_array<5>  firmsID;
    t_array<20> devTag;
    t_array<7>  devID;
	t_uint8	    carColor;	 //1-blue, 2-yellow, 3-black, 4-white, 9-other, 0-none
	t_string    carIdentity; //car VIN if color is 0, if not, car license

 	DECLARE_MESSAGE(MSG808_DEV_REGISTER_REQ);
};

struct register_reply
{
	t_uint16 reqSeq;
	t_uint8  result;	//0-OK, 1-car registered, 2-Without the car, 3-terminal registered, 4-without the terminal
	t_string authCode;  //invalid if result is 0

 	DECLARE_MESSAGE(MSG808_DEV_REGISTER_RSP);
};

struct unregister_req
{
 	DECLARE_MESSAGE(MSG808_DEV_UNREGISTER);
};

struct authenticate_req
{
	t_string authCode;

 	DECLARE_MESSAGE(MSG808_DEV_AUTHORISE);
};

struct parameter 
{
    t_uint32 id;
    t_string value;

    DECLARE_SERIALIZE();
};

struct set_param_req
{
    std::vector<parameter> params;

 	DECLARE_MESSAGE(MSG808_SET_DEV_PARAM_REQ);
};

struct get_all_param_req
{
 	DECLARE_MESSAGE(MSG808_QUERY_DEV_ALL_PARAM_REQ);
};

struct get_part_param_req
{
	std::vector<t_uint32> paramIDs;	

	DECLARE_MESSAGE(MSG808_QUERY_DEV_PART_PARAM_REQ);
};

struct get_param_reply
{
	t_uint16 reqSeq;
    std::vector<parameter> params;

 	DECLARE_MESSAGE(MSG808_QUERY_DEV_PARAM_RSP);
};

struct control_dev_req
{
    t_uint8  cmdID;
    t_string cmdValue;

    DECLARE_MESSAGE(MSG808_DEV_CONTROL_REQ);
};

struct property
{
	t_uint16    devType;
    t_array<5>  firmsID;
    t_array<20> devTag;
    t_array<7>  devID;
    t_array<20> simICCID;
	t_string    hardware;
	t_string    firmware;
	t_uint8     gnssAttr;
	t_uint8	    linkAttr;

    DECLARE_SERIALIZE();
};

struct get_property_req
{
 	DECLARE_MESSAGE(MSG808_QUERY_DEV_PROPERTY_REQ);
};

struct get_property_reply
{
	property attr;

 	DECLARE_MESSAGE(MSG808_QUERY_DEV_PROPERTY_RSP);
};

struct upgrade_req
{
    t_uint8    type;
    t_array<5> firmsID;
    t_string   version;
    t_uint32   packetSize;
    t_buffer   packetData;

    DECLARE_MESSAGE(MSG808_DEV_UPGRADE_REQ);
};

struct upgrade_reply
{
    t_uint8 type;
    t_uint8 result;

    DECLARE_MESSAGE(MSG808_DEV_UPGRADE_RSP);
};

struct annex
{
    t_uint8  id;
    t_string value; //use ';' separate multiple values 

    DECLARE_SERIALIZE();
};

struct gps
{
	t_uint32 alarmType;
	t_uint32 status;
	double   latitude;
	double   longitude;
	t_uint16 elevation;
	t_uint16 speed;		//units: 1/10km/h 
	t_uint16 direction;	//0-359£¬north is 0
	t_string time;	
    t_vector<annex> annexs;
	
    DECLARE_SERIALIZE();
};

struct annexed_overspeed
{
    t_uint8  siteType;     // 0:No specify position, 1:Circular area, 2:Rectangle area, 3:Polygon area, 4:Route
    t_uint32 areaID;       // invalid if siteType is 0

    DECLARE_SERIALIZE();
};

struct annexed_arealarm
{
    t_uint8  siteType;  // 1:Circular area, 2:Rectangle area, 3:Polygon area, 4:Route
    t_uint32 areaID;
    t_uint8  direction; // 0:in, 1:out

    DECLARE_SERIALIZE();
};

struct annexed_traveltime
{
    t_uint32 areaID;
    t_uint16 travelTime;  // units: second
    t_uint8  result;      // 0:lack, 1:too long

    DECLARE_SERIALIZE();
};

struct report_gps
{
	gps site;

 	DECLARE_MESSAGE(MSG808_DEV_GPS_REPORT);
};

struct get_gps_req
{
 	DECLARE_MESSAGE(MSG808_QUERY_DEV_GPS_REQ);
};

struct get_gps_reply
{
	t_uint16 reqSeq;
	gps      site;

 	DECLARE_MESSAGE(MSG808_QUERY_DEV_GPS_RSP);
};

struct trace_gps_req
{
    t_uint16 interval;  // unit: second
    t_uint16 expiry;    // unit: second

    DECLARE_MESSAGE(MSG808_DEV_TRACE_REQ);
};

struct verify_alarm
{
    t_uint16 reqSeq; // 0 - verify all alarm
    t_uint32 alarmType;

    DECLARE_MESSAGE(MSG808_VERIFY_ALARM);
};

struct notify_req
{
	t_uint8  textFlag;
	t_string text;

 	DECLARE_MESSAGE(MSG808_NOTIFY_DEV);
};

struct get_driver_req
{
 	DECLARE_MESSAGE(MSG808_QUERY_DRIVER_REQ);
};

struct get_driver_reply
{
    t_uint8	    status;      //0x01: clock in , 0x02: clock out
    t_string    time;        //format: YYMMDDhhmmss 
    t_uint8	    flag;        //0x00: read card ok, 0x01: pwd err, 0x02: card locked, 0x03: card be taken, 0x04: data check err
	t_string    name;
	t_array<20> license; 
	t_string    licensor;
    t_string    expiredTime; //"YYYYMMDD"

 	DECLARE_MESSAGE(MSG808_QUERY_DRIVER_RSP);
};

struct call_phone_req
{
	t_uint8  flag;  //0-normal, 1-monitor
	t_string phone; // size must less than 20

 	DECLARE_MESSAGE(MSG808_PHONE_CALLBACK);
};

struct set_record_req
{
    t_uint8  cmd;        //0-stop,  1-start
    t_uint16 duration;   //0-all the time, units: seconds
    t_uint8  saveFlag;   //0-In real time to upload, 1-save
	t_uint8  sampleRate; //0-8K, 1-11K, 2-23K, 3-32K

 	DECLARE_MESSAGE(MSG808_SET_RECORD);
};

struct up_tunnel_data
{
    t_uint8  type;
    t_vector<t_uint8> content;

    DECLARE_MESSAGE(MSG808_UP_TUNNEL_DATA);
};

struct down_tunnel_data
{
    t_uint8  type;
    t_vector<t_uint8> content;

    DECLARE_MESSAGE(MSG808_DOWN_TUNNEL_DATA);
};


} 
