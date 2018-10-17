#include "wifi_setup.h"
#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WebServer.h>
#include <user_interface.h>

#include "main.h"

//CONNECT TP-LINK_E6001A 995760ROBERT
//CONFIG NTP 150.254.183.15 RRRR-MM-DD_gg:mm:ss 1 1
//CONFIG TIME http://api.geonames.org/timezone?lat=54&lng=0&username=robi99956&style=full <time> RRRR-MM-DD_gg:mm:ss 1 1
//CONFIG LOCTIME http://ip-api.com/xml http://api.geonames.org/timezone?lat=$lat&lng=$lon&username=robi99956&style=full <lon> <lat> <time> RRRR-MM-DD_gg:mm:ss

String loc_url;
String time_url;
IPAddress ntp_ip;
int gmt_offset;
bool uzyj_latozima;

String time_mark;
String lon_mark;
String lat_mark;

String time_format;

get_time_mode time_mode;

ESP8266WiFiAPClass AP;

char ssid[40], pass[40];

void connect_to_wifi( char * ssid_pass, bool legal_call )
{
	if( legal_call == 0 )
	{
		uart_send_std("CONNECT"); // @suppress("Invalid arguments")
		return;
	}

	char * _ssid = strtok(ssid_pass, " ");
	char * _pass = strtok(NULL, " ");

	strcpy(ssid, _ssid);
	strcpy(pass, _pass);
#if 1
	static ap_state state=OFF;

	WiFi.disconnect(1);
	WiFi.begin(ssid, pass);

	uint32_t time = millis();
	wl_status_t status;

	// Wait for connection
	while( (status=WiFi.status()) != WL_CONNECTED )
	{
		if( status == WL_CONNECT_FAILED || millis()-time > 10000 )
		{
			uart_send_std("CONNECT ERROR"); // @suppress("Invalid arguments")

			if( state == OFF )
			{
				WiFi.disconnect(1);
				WiFi.mode( WIFI_AP_STA );

				AP.softAPConfig(IPAddress(192, 168, 1, 1), IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));
				AP.softAP("zegarek");
				state = ON;
			}

			return;
		}

		delay(200);
	}

	if( state == ON )
	{
		AP.softAPdisconnect();
		WiFi.mode(WIFI_STA);
		state = OFF;
	}

	uart_send_std("CONNECT OK"); // @suppress("Invalid arguments")
#else
	WiFi.disconnect(1);
	WiFi.begin(ssid, pass);

	uart_send_std("CONNECT OK"); // @suppress("Invalid arguments")
#endif
}

void reconnect_event( void )
{
	station_status_t status = wifi_station_get_connect_status();
	static bool ap_on=0;

	if( status != STATION_CONNECTING && status != STATION_GOT_IP )
	{
		WiFi.disconnect(1);
		WiFi.begin(ssid, pass);

		if( ap_on == 0 )
		{
			WiFi.mode(WIFI_AP_STA);
			AP.softAPConfig(IPAddress(192, 168, 1, 1), IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));
			AP.softAP("zegarek");
			ap_on = 1;
		}
	}
	else
		if( status == STATION_GOT_IP )
		{
			if( AP.softAPgetStationNum() == 0 )
			{
				AP.softAPdisconnect(1);
				WiFi.mode(WIFI_STA);
				ap_on = 0;
			}
		}

}

IPAddress get_ip( char * ip )
{
  uint8_t a, b, c, d;

   a = atoi( strtok(ip, ".") );
   b = atoi( strtok(NULL, ".") );
   c = atoi( strtok(NULL, ".") );
   d = atoi( strtok(NULL, ".") );

   return IPAddress(a, b, c, d);
}

void set_config( char * conf, bool legal_call )
{
	if( legal_call == 0 )
	{
		uart_send_std("CONFIG"); // @suppress("Invalid arguments")
		return;
	}

	char * mode = strtok(conf, " ");

	WSK_CHECK(mode); // @suppress("Invalid arguments")

	if( strcmp(mode, "MANUAL") == 0 ) // bez niczego
	{
		time_mode = MANUAL;
	}
	else
	if( strcmp(mode, "NTP") == 0 ) // IP FORMAT OFFSET UZYJ_LATOZIMA
	{
		time_mode = NTP;
		char * ip = strtok(NULL, " ");
		WSK_CHECK(ip); // @suppress("Invalid arguments")

		char * pattern = strtok(NULL, " ");
		WSK_CHECK(pattern); // @suppress("Invalid arguments")
		time_format = String(pattern);

		char * offset = strtok(NULL, " ");
		WSK_CHECK(offset); // @suppress("Invalid arguments")
		gmt_offset = atoi(offset);

		char * latozima = strtok(NULL, " ");
		WSK_CHECK(latozima);
		uzyj_latozima = atoi(latozima);

		ntp_ip = get_ip(ip);
	}
	else
	  if( strcmp(mode, "TIME") == 0 ) // URL ZNACZNIK FORMAT OFFSET UZYJ_LATOZIMA
	  {
		time_mode = API_TIME;

		char * url = strtok(NULL, " ");
		WSK_CHECK(url); // @suppress("Invalid arguments")
		time_url = String(url);

		char * mark = strtok(NULL, " ");
		WSK_CHECK(mark); // @suppress("Invalid arguments")
		time_mark = String(mark);

		char * pattern = strtok(NULL, " ");
		WSK_CHECK(pattern); // @suppress("Invalid arguments")
		time_format = String(pattern);

		char * offset = strtok(NULL, " ");
		WSK_CHECK(offset); // @suppress("Invalid arguments")
		gmt_offset = atoi(offset);

		char * latozima = strtok(NULL, " ");
		WSK_CHECK(latozima); // @suppress("Invalid arguments")
		uzyj_latozima = atoi(latozima);
	  }
	  else
		if( strcmp(mode, "LOCTIME") == 0 ) // URL_LOC URL_CZAS ZNACZNIK_LON ZNACZNIK_LAT ZNACZNIK_CZAS FORMAT
		{
		  time_mode = API_LOCTIME;

		  char * url = strtok(NULL, " ");
		  WSK_CHECK(url); // @suppress("Invalid arguments")
		  loc_url = String(url);

		  url = strtok(NULL, " ");
		  WSK_CHECK(url); // @suppress("Invalid arguments")
		  time_url = String(url);

		  char * mark = strtok(NULL, " ");
		  WSK_CHECK(mark); // @suppress("Invalid arguments")
		  lon_mark = String(mark);

		  mark = strtok(NULL, " ");
		  WSK_CHECK(mark); // @suppress("Invalid arguments")
		  lat_mark = String(mark);

		  mark = strtok(NULL, " ");
		  WSK_CHECK(mark); // @suppress("Invalid arguments")
		  time_mark = String(mark);

		  char * pattern = strtok(NULL, " ");
		  WSK_CHECK(pattern); // @suppress("Invalid arguments")
		  time_format = String(pattern);

		  gmt_offset = 0;
		}

		uart_send_std("OK"); // @suppress("Invalid arguments")
}

void get_status( bool legal_call )
{
	String stat = "STATUS ";

	if( legal_call )
	{
		if( WiFi.status() != WL_CONNECTED ) stat += "DISCONNECTED";
		else stat += "CONNECTED";
	}

	uart_send_std( stat.c_str() ); // @suppress("Invalid arguments")
}

void get_ip( bool legal_call )
{
	if( legal_call )
	{
		if( WiFi.status() == WL_CONNECTED ) uart_send_std( ("IP " + WiFi.localIP().toString()).c_str() ); // @suppress("Invalid arguments")
		else uart_send_std( "IP DISCONNECTED" ); // @suppress("Invalid arguments")
	}
	else
		uart_send_std("IP");
}

void scan_callback( int ile )
{
	uart_send_std("WIFI LIST START"); // @suppress("Invalid arguments")
	delay(10);

	for( uint16_t i=0; i<ile; i++ )
	{
	uart_send_std( ("WIFI " + WiFi.SSID(i)).c_str() ); // @suppress("Invalid arguments") // @suppress("Method cannot be resolved")
	delay(10);
	}

	uart_send_std("WIFI LIST END"); // @suppress("Invalid arguments")
}

void wifi_scan( bool legal_call )
{
	if( legal_call ) WiFi.scanNetworksAsync(scan_callback, 0);
}



