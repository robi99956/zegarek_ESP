#include <ESP8266HTTPClient.h>
#include <WiFiUdp.h>
#include "time.h"
#include "wifi_setup.h"

CZAS czas;

bool client_GET( const char * url, String &dane )
{
	if( WiFi.status() != WL_CONNECTED ) return 0;

	HTTPClient http;
	bool retval=1;

	http.begin(url);

	int code = http.GET();

	if( code == HTTP_CODE_OK )
	{
		dane = http.getString();
	}
	else
		retval = 0;

	http.end();

	return retval;
}

void get_time( bool legal_call )
{
	String czas = "TIME ";

	if( legal_call )
	{
		switch( time_mode )
		{
		case MANUAL:
			czas += "MANUAL";
			break;

		case NTP:
			czas += get_time_NTP();
			break;

		case API_TIME:
			czas += get_time_TIMEAPI();
			break;

		case API_LOCTIME:
			czas += get_time_LOCTIMEAPI();
			break;
		}
	}

	uart_send_std( czas.c_str() );
}

String get_time_NTP( void )
{
	WiFiUDP udp; // @suppress("Abstract class cannot be instantiated")
	udp.begin(2390);
	uint8_t packetBuffer[48];

	memset(packetBuffer, 0, 48);

	packetBuffer[0] = 0b11100011;   // LI, Version, Mode
	packetBuffer[1] = 0;     // Stratum, or type of clock
	packetBuffer[2] = 6;     // Polling Interval
	packetBuffer[3] = 0xEC;  // Peer Clock Precision
	// 8 bytes of zero for Root Delay & Root Dispersion
	packetBuffer[12]  = 49;
	packetBuffer[13]  = 0x4E;
	packetBuffer[14]  = 49;
	packetBuffer[15]  = 52;

	// all NTP fields have been given values, now
	// you can send a packet requesting a timestamp:
	udp.beginPacket(ntp_ip, 123); //NTP requests are to port 123
	udp.write(packetBuffer, 48);
	udp.endPacket();

	uint16_t cnt=0;

	while( udp.parsePacket() == 0 )
	{
		delay(1);

		if( cnt++ > 3000 ) return "ERROR";
	}

	udp.read(packetBuffer, 48);
	uint16_t highWord = word(packetBuffer[40], packetBuffer[41]);
	uint16_t lowWord = word(packetBuffer[42], packetBuffer[43]);

	czas.czas_unix = highWord << 16 | lowWord;

	gmtime(&czas);

	char buf[30] = {0};
	czas_to_txt(buf, time_format.c_str(), &czas);

	return String(buf);
}

void add_gmt_offset( String &time_str )
{
	CZAS t;
	memset(&t, 0, sizeof(CZAS));

	txt_to_czas(time_str.c_str(), time_format.c_str(), &t);
	uwzglednij_gmt_offset(&t, gmt_offset);

	char buf[30] = {0};
	czas_to_txt(buf, time_format.c_str(), &t);

	time_str = String(buf);
}

String get_time_TIMEAPI( void )
{
	String dane;
	String retval = "ERROR";

	if( client_GET(time_url.c_str(), dane) )
	{
		char time_str[30];
		char * buf = (char*)dane.c_str();

		if( is_xml(buf) ) // format - xml
		{
			if( xml_find( buf, time_mark.c_str(), time_str, sizeof(time_str) ) )
			{
				retval = String(time_str);
				add_gmt_offset(retval);
			}
		}
		else
			if( is_json(buf) )
			{
				if( json_find( buf, time_mark.c_str(), time_str, sizeof(time_str) ) )
				{
					retval = String(time_str);
					add_gmt_offset(retval);
				}
			}
	}

	return retval;
}

bool get_lat_lon( char * lat, char * lon )
{
	String dane;

	if( client_GET(loc_url.c_str(), dane) )
	{
		char * buf = (char*)dane.c_str();

		if( is_xml(buf) ) // format - xml
		{
			if( xml_find( buf, lat_mark.c_str(), lat, 10 ) == 0 ) return 0;
			if( xml_find( buf, lon_mark.c_str(), lon, 10 ) == 0 ) return 0;
		}
		else
		if( is_json(buf) ) // format - json
		{
			if( json_find( buf, lat_mark.c_str(), lat, 10 ) == 0 ) return 0;
			if( json_find( buf, lon_mark.c_str(), lon, 10 ) == 0 ) return 0;
		}

		return 1;
	}

	return 0;
}

String get_time_LOCTIMEAPI( void )
{
	char lat[10], lon[10];

	if( get_lat_lon(lat, lon) )
	{
		String _time_url = time_url;

		time_url.replace("$lat", String(lat));
		time_url.replace("$lon", String(lon));

		String retval = get_time_TIMEAPI();

		time_url = _time_url;

		return retval;
	}

	return "ERROR";
}


