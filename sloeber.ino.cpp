#ifdef __IN_ECLIPSE__
//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2018-10-18 01:45:14

#include "Arduino.h"
#include <ESP8266HTTPClient.h>
#include <WiFiUdp.h>
#include "time.h"
#include "wifi_setup.h"
#include "wifi_setup.h"
#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WebServer.h>
#include <user_interface.h>
#include "main.h"
#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <pgmspace.h>
#include <queue>
#include <vector>
#include "main.h"
extern ESP8266WebServer server;
extern std::queue<String> post_requests;
extern std::vector<http_file_t> http_files;
#define ALIGN     __attribute__ (( aligned ( 4 ) ))
#define INFLASH   PROGMEM ALIGN
extern const int8_t jquery_min[];
extern const int8_t jquery_min[];
extern const int8_t jquery_min[];
extern const int8_t jquery_min[];
extern const int8_t bootstrap_js[];
extern const int8_t bootstrap_js[];
extern const int8_t bootstrap_js[];
extern const int8_t bootstrap_js[];
extern const int8_t bootstrap_css[];
extern const int8_t bootstrap_css[];
extern const int8_t bootstrap_css[];

bool client_GET( const char * url, String &dane ) ;
void get_time( bool legal_call ) ;
String get_time_NTP( void ) ;
void add_gmt_offset( String &time_str ) ;
String get_time_TIMEAPI( void ) ;
bool get_lat_lon( char * lat, char * lon ) ;
String get_time_LOCTIMEAPI( void ) ;
uint8_t json_find(char * str, const char * marker, char * dst, uint16_t maxlen) ;
uint8_t is_json(char * str) ;
uint8_t xml_find(char * str, const char * marker, char * dst, uint16_t maxlen) ;
uint8_t is_xml(char * str) ;
void uart_parse( String &dane, bool is_main_loop ) ;
void uart_send_std( const char * dane ) ;
void connect_to_wifi( char * ssid_pass, bool legal_call ) ;
void reconnect_event( void ) ;
IPAddress get_ip( char * ip ) ;
void set_config( char * conf, bool legal_call ) ;
void get_status( bool legal_call ) ;
void get_ip( bool legal_call ) ;
void scan_callback( int ile ) ;
void wifi_scan( bool legal_call ) ;
void setup(void) ;
void loop(void) ;
void serialEvent( bool is_main_loop ) ;
uint32_t flash_read( const void *src, void *dst, uint32_t len ) ;
void send_post_request( void ) ;
void read_file_from_uart( char * cmd ) ;
http_file_t * get_file_by_path( String path ) ;

#include "zegar.ino"

#include "client.ino"
#include "parsers.ino"
#include "uart_parse.ino"
#include "wifi_setup.ino"

#endif
