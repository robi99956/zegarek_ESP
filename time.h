/*********************************************
 * vim:sw=8:ts=8:si:et
 * To use the above modeline in vim you must have "set modeline" in your .vimrc
 * Author: Guido Socher 
 * Copyright: GPL V2
 *
 *********************************************/
//@{
#ifndef TIMECONVERSIONS_H
#define TIMECONVERSIONS_H

#include <stdint.h>
// Number of seconds between 1-Jan-1900 and 1-Jan-1970, unix time starts 1970
// and ntp time starts 1900.
#define GETTIMEOFDAY_TO_NTP_OFFSET 2208988800UL

typedef struct
{
  uint8_t sek;
  uint8_t min;
  uint8_t godz;
  uint8_t dzien;
  uint8_t mies;
  uint16_t rok;
  uint8_t dzien_tygodnia;
  uint32_t czas_unix;
} CZAS;

extern void gmtime( CZAS * czas );
void txt_to_czas( const char * buf, const char * pattern, CZAS * czas );
void czas_to_txt( char * buf, const char * pattern, CZAS * czas );
void uwzglednij_gmt_offset( CZAS * czas, int offset );

#endif /* TIMECONVERSIONS_H */
//@}





