/*
 * typedef.h
 *
 *  Created on: 18 paü 2018
 *      Author: Robert
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "Arduino.h"

typedef struct _http_file
{
	String path;
	char * data;
} http_file_t;

#define WSK_CHECK(wsk) if(wsk==NULL){uart_send_std((const char*)"ERROR");return;}

#endif /* MAIN_H_ */
