#ifndef WIFI_SETUP_H
#define WIFI_SETUP_H

enum get_time_mode{ MANUAL, NTP, API_TIME, API_LOCTIME };
enum ap_state{ OFF, ON };

extern int gmt_offset;
extern get_time_mode time_mode;
extern bool uzyj_latozima;

#ifndef EXTERNAL_CODE
extern String loc_url;
extern String time_url;
extern IPAddress ntp_ip;

extern String time_mark;
extern String lon_mark;
extern String lat_mark;

extern String time_format;
#endif

#endif




