#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <pgmspace.h>
#include <queue>
#include <vector>

#include "Scheduler/scheduler.h"
#include "main.h"

void serialEvent( void );
void uart_send_std( const char * dane );
uint32_t flash_read( const void *src, void *dst, uint32_t len );
http_file_t * get_file_by_path( String path );

//CONNECT TP-LINK_E6001A 995760ROBERT
//CONNECT Fiszera 00000111111

ESP8266WebServer server(80);

std::queue<String> post_requests;
std::vector<http_file_t> http_files;

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


	bool handle (ESP8266WebServer &server, HTTPMethod requestMethod, String requestUri)
	{
		if( requestMethod == HTTP_GET )
		{
			if( process_LIB(requestUri) ) return 1;

			uart_send_std(requestUri.c_str());

			http_file_t * file = get_file_by_path(requestUri);
			if( file == NULL ) return 0;

			String dataType = "text/plain";
			if(requestUri.endsWith(".htm") || requestUri.endsWith(".html") || requestUri.endsWith("/") ) dataType = "text/html";
			else if(requestUri.endsWith(".css")) dataType = "text/css";
			else if(requestUri.endsWith(".js")) dataType = "application/javascript";
			else if(requestUri.endsWith(".png")) dataType="image/png";

			server.send(200, dataType, String(file->data));

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
	scheduler_pool();
	server.handleClient();
	serialEvent();
}

void serialEvent( void )
{
  static String wejscie_str;

  while( Serial.available() )
  {
    char inChar = (char)Serial.read();

    if( inChar == '\r' )
    {
      uart_parse(wejscie_str); // @suppress("Invalid arguments")

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

void read_file_from_uart( char * cmd )
{
	char * path = strtok(cmd, " ");
	WSK_CHECK(path);

	if( get_file_by_path(path) ) return;

	char * _len = strtok(NULL, " ");
	WSK_CHECK(_len);

	http_file_t new_file;

	uint16_t len = atoi(_len);
	new_file.data = (char*)calloc(len+1, 1);
	
	if( new_file.data == NULL )
	{
		uart_send_std("FILE ERROR");
		return;
	}

	new_file.data[ len ] = 0;

	new_file.path = String(path);

	uart_send_std("FILE READY");

	size_t read_len = Serial.readBytesUntil('\r', new_file.data, len);

	http_files.push_back(new_file);

	if( read_len == len ) uart_send_std("FILE OK");
	else uart_send_std("FILE ERROR");
}

http_file_t * get_file_by_path( String path )
{
	size_t len = http_files.size();

	while(len--)
	{
		if( http_files[len].path == path )
		{
			return &(http_files.data()[len]);
		}
	}

	return NULL;
}
