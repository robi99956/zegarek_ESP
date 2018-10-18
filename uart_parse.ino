#define STD_LEN 200

char connect_str[] = "CONNECT";
char time_str[] = "TIME";
char config_str[] = "CONFIG";
char status_str[] = "STATUS";
char scan_str[] = "SCAN";
char ip_str[] = "GETIP";
char reset_str[] = "RESET";
char file_str[] = "FILE";
char get_post_requests_str[] = "POST";

void uart_parse( String &dane )
{
	char * c_dane = (char*)calloc(dane.length()+1, 1);
	strcpy(c_dane, dane.c_str());

	if( strncmp(c_dane, connect_str, sizeof(connect_str)-1) == 0 )
		connect_to_wifi( c_dane+sizeof(connect_str) );
	else
		if( strncmp(c_dane, time_str, sizeof(time_str)-1) == 0 )
		get_time();
	else
		if( strncmp(c_dane, config_str, sizeof(config_str)-1) == 0 )
		set_config(c_dane + sizeof(config_str) );
	else
		if( strncmp(c_dane, status_str, sizeof(status_str)-1) == 0 )
		get_status();
	else
		if( strncmp(c_dane, scan_str, sizeof(scan_str)-1) == 0 )
		wifi_scan();
	else
		if( strncmp(c_dane, ip_str, sizeof(ip_str)-1) == 0 )
		get_ip();
	else
		if( strncmp(c_dane, reset_str, sizeof(reset_str)-1) == 0 )
		ESP.restart();
	else
		if( strncmp(c_dane, file_str, sizeof(file_str)-1) == 0 )
		read_file_from_uart( c_dane + sizeof(file_str) );
	else
		if( strncmp(c_dane, get_post_requests_str, sizeof(get_post_requests_str)-1) == 0 )
		send_post_request();

	free(c_dane);
}

void uart_send_std( const char * dane )
{
  uint8_t len = strlen(dane);

  for( uint8_t i=0; i<STD_LEN; i++ )
  {
    if( i < len ) Serial.write(dane[i]);
    else Serial.write(0);
  }
}





