#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <pgmspace.h>
#include <queue>

void serialEvent( bool );
void uart_send_std( const char * dane );
uint32_t flash_read( const void *src, void *dst, uint32_t len );

//CONNECT TP-LINK_E6001A 995760ROBERT
//CONNECT Fiszera 00000111111

ESP8266WebServer server(80);

char strona_buf[15000];
String get_request;
size_t read_data_len;
std::queue<String> post_requests;

#define ALIGN     __attribute__ (( aligned ( 4 ) ))
#define INFLASH   PROGMEM ALIGN

const int8_t jquery_min[] INFLASH =
{
		#include "jquery.tab"
};

const int8_t bootstrap_js[] INFLASH =
{
		#include "bootstrap_js.tab"
};

const int8_t bootstrap_css[] INFLASH =
{
		#include "bootstrap_css.tab"
};

const char content_js[] INFLASH = "application/javascript";
const char content_css[] INFLASH = "text/css";

class handler : public RequestHandler
{
	bool canHandle( HTTPMethod method, String uri ){ (void)uri; return (method==HTTP_GET || method==HTTP_POST);};
	bool canUpload (String uri){ (void)uri; return 0;};

	bool process_LIB( String &uri )
	{
		if( uri == "/LIB/jquery-3.3.1.min.js" )
		{
			server.send_P(200, content_js, (const char*)jquery_min);
			return 1;
		}
		else
		if( uri == "/LIB/bootstrap.min.js" )
		{
			server.send_P(200, content_js, (const char*)bootstrap_js);
			return 1;
		}
		else
		if( uri == "/LIB/bootstrap.min.css" )
		{
			server.send_P(200, content_css, (const char*)bootstrap_css);
			return 1;
		}

		return 0;
	}

	size_t wait_for_pool( String requestUri )
	{
		get_request = requestUri;

		uint32_t start_tick = millis();

		while( millis() - start_tick < 5000 && read_data_len == 0 )
		{
			serialEvent(0);
		}

		size_t retval = read_data_len;
		read_data_len=0;

		return retval;
	}

	bool handle (ESP8266WebServer &server, HTTPMethod requestMethod, String requestUri)
	{
		if( requestMethod == HTTP_GET )
		{
			if( process_LIB(requestUri) ) return 0;

			size_t len = wait_for_pool(requestUri);

			if( len == 0 ) return 0;

			strona_buf[len] = 0;

			String dataType = "text/plain";
			if(requestUri.endsWith(".htm") || requestUri.endsWith(".html") || requestUri.endsWith("/") ) dataType = "text/html";
			else if(requestUri.endsWith(".css")) dataType = "text/css";
			else if(requestUri.endsWith(".js")) dataType = "application/javascript";
			else if(requestUri.endsWith(".png")) dataType="image/png";

			server.send(200, dataType, strona_buf);

			return 1;
		}
		else
		if( requestMethod == HTTP_POST )
		{
			server.send(200, "text/plain", "Zmiany zapisane!");
			String dane;

			uint8_t argc = server.args();

			for( uint8_t i=0; i<argc; i++ )
			{
				dane = "POST " + server.argName(i) + "=" + server.arg(i);
				post_requests.push(dane);
			}

			return 1;
		}

		return 0;
	}
};

handler zapytania;

void setup(void)
{
  WiFi.mode(WIFI_STA);

  Serial.begin(115200);
  Serial.setTimeout(10000);

  server.addHandler( &zapytania );

  server.begin();

  WiFi.disconnect();
}

void loop(void)
{
	reconnect_event(); // @suppress("Invalid arguments")
	server.handleClient();
	serialEvent(1);
}

void serialEvent( bool is_main_loop )
{
  static String wejscie_str;

  while( Serial.available() )
  {
    char inChar = (char)Serial.read();

    if( inChar == '\r' )
    {
      uart_parse(wejscie_str, is_main_loop); // @suppress("Invalid arguments")

      wejscie_str = "";
      return;
    }

    wejscie_str += inChar;
  }
}

uint32_t flash_read( const void *src, void *dst, uint32_t len )
{
	len += len%4;

	memcpy(dst, src, len);

	return len;
}

void send_get_request( void )
{
	if( get_request.length() > 0 )
	{
		uart_send_std( ("GET " + get_request).c_str() );
		get_request = "";

		read_data_len = Serial.readBytesUntil('\r', strona_buf, sizeof(strona_buf)-1);
	}
	else
	{
		uart_send_std( "GET NONE" );
	}
}

void send_post_request( void )
{
	if( post_requests.empty() )
	{
		uart_send_std( "POST NONE" );
	}
	else
	{
		uart_send_std("POST START");
		delay(10);

		while( post_requests.empty() == 0 )
		{
			uart_send_std( post_requests.front().c_str() );
			post_requests.pop();

			delay(10);
		}

		uart_send_std("POST END");
	}
}



