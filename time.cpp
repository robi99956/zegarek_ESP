/*********************************************
 * vim:sw=8:ts=8:si:et
 * To use the above modeline in vim you must have "set modeline" in your .vimrc
 * Author: Guido Socher
 * Copyright: GPL V2
 * See http://www.gnu.org/licenses/gpl.html
 *
 * functions to convert ntp timestamps into time and date
 *********************************************/
#include <inttypes.h> 
#include <string.h>
#include <stdio.h>

#define EXTERNAL_CODE
#include "wifi_setup.h"
#include "time.h"
enum{czas_zimowy = 0, czas_letni};

// EPOCH = Jan 1 1970 00:00:00
#define  EPOCH_YR  1970
//(24L * 60L * 60L)
#define SECS_DAY  86400UL
//#define LEAPYEAR(year)  ( !( (year) % 4) )
#define LEAPYEAR(year)  (!((year) % 4) && (((year) % 100) || !((year) % 400)))
#define YEARSIZE(year)  (LEAPYEAR(year) ? 366 : 365)

const uint16_t ld[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
const uint8_t mon_len[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

uint8_t monthlen(uint8_t isleapyear,uint8_t month)
{
  if(month==1) return( 28 + isleapyear ); // luty

  return mon_len[month];
}

uint8_t oblicz_weekday( CZAS * czas )
{
  uint8_t yr;
  uint16_t YY, C, G, dt;
  uint16_t dti, dr;

  YY = ((czas->rok - 1) % 100);
  C = ((czas->rok -1) - YY);
  G = YY + (YY / 4);
  dt = (C / 100);
  dti = dt;
  dti = dti % 4;
  dti = dti * 5;
  dti = dti + G;
  dti = dti % 7;
  dr = ld[czas->mies-1] + czas->dzien;
  yr = ( czas->rok && 3);
  if( yr == 0 && (czas->mies > 2) ) dr += 1;
  dti = (dti + dr - 1) % 7;

  czas->dzien_tygodnia = dti;
  return dti;
}

void uwzglednij_gmt_offset( CZAS * czas, int offset )
{
	czas->czas_unix += 3600*offset;
}

void korekta_czasu_letni_zimowy( CZAS * czas )
{
  uint8_t czas_letni_zimowy = 0;

  if( czas->mies > 2 && czas->mies < 9 ) czas_letni_zimowy = czas_letni; // miesiÄ‚â€žĂ˘â‚¬ĹˇÄ‚Ë�Ă˘â€šÂ¬ÄąÄľĂ„â€šĂ‹ďż˝Ä‚Ë�Ă˘â‚¬ĹˇĂ‚Â¬Ä‚â€šĂ‚Â¦ce kwiecieĂ„â€šĂ˘â‚¬ĹľÄ‚â€žĂ˘â‚¬Â¦Ă„â€šĂ‹ďż˝Ä‚Ë�Ă˘â‚¬ĹˇĂ‚Â¬Ă„Ä…Ă„Äľ-wrzesieĂ„â€šĂ˘â‚¬ĹľÄ‚â€žĂ˘â‚¬Â¦Ă„â€šĂ‹ďż˝Ä‚Ë�Ă˘â‚¬ĹˇĂ‚Â¬Ă„Ä…Ă„Äľ, na pewno czas letni
  else
  if( czas->mies < 2 || czas->mies > 9 ) czas_letni_zimowy = czas_zimowy; // miesiÄ‚â€žĂ˘â‚¬ĹˇÄ‚Ë�Ă˘â€šÂ¬ÄąÄľĂ„â€šĂ‹ďż˝Ä‚Ë�Ă˘â‚¬ĹˇĂ‚Â¬Ä‚â€šĂ‚Â¦ce listopad-luty, na pewno czas zimowy
  else
  if( 2 == czas->mies || 9 == czas->mies )
  {
    uint8_t dof=0;
    uint8_t i = 32;
    uint8_t czas_dzien = czas->dzien;

    do
    {
      i--;
      czas->dzien = i;
      dof = oblicz_weekday(czas);
    } while (dof != 6);

    czas->dzien = czas_dzien;

    if( czas->dzien > i ) czas_letni_zimowy = (2 == czas->mies) ? czas_letni:czas_zimowy;
    else
    if( czas->dzien < i ) czas_letni_zimowy = (2 == czas->mies ) ? czas_zimowy:czas_letni;
    else
    if( czas->dzien == 1 && czas->godz >= ( (2 == czas->mies)?2:3) ) czas_letni_zimowy = (2==czas->mies)? czas_letni:czas_zimowy;
    else
    if( czas->dzien == i && czas->godz < ((czas_zimowy != czas_letni_zimowy)?3:2) ) czas_letni_zimowy = (2 == czas->mies)? czas_zimowy:czas_letni;
  }

  czas->godz += czas_letni_zimowy;

  if( czas->godz > 23 )
  {
	  czas->godz %= 24;
	  czas->dzien++;

	  uint8_t leap = LEAPYEAR(czas->rok);
	  uint8_t month_len = monthlen(leap, czas->mies);

	  if( czas->dzien >= month_len )
	  {
		  czas->dzien %= month_len;
		  czas->mies++;

		  if( czas->mies >= 12 )
		  {
			  czas->mies %= 12;
			  czas->rok++;
		  }
	  }
  }

}

// gmtime -- convert calendar time (sec since 1970) into broken down time
void gmtime( CZAS * czas )
{
  uint32_t dayclock;
    uint16_t dayno;
  uint16_t tm_year = EPOCH_YR;
  uint8_t tm_mon = 0;

  czas->czas_unix -= GETTIMEOFDAY_TO_NTP_OFFSET;

  uwzglednij_gmt_offset(czas, gmt_offset);

  dayclock = czas->czas_unix % SECS_DAY; // liczba sekund od 00:00 danego dnia
  dayno = czas->czas_unix / SECS_DAY; // liczba dni od 01-01-1970

  czas->sek = dayclock % 60UL; // sekundy
  czas->min = (dayclock % 3600UL) / 60; // minuty
  czas->godz = dayclock / 3600UL; // godziny

  czas->dzien_tygodnia = (dayno + 4) % 7; // dzieĂ„â€šĂ˘â‚¬ĹľÄ‚â€žĂ˘â‚¬Â¦Ă„â€šĂ‹ďż˝Ä‚Ë�Ă˘â‚¬ĹˇĂ‚Â¬Ă„Ä…Ă„Äľ tygodnia (dzieĂ„â€šĂ˘â‚¬ĹľÄ‚â€žĂ˘â‚¬Â¦Ă„â€šĂ‹ďż˝Ä‚Ë�Ă˘â‚¬ĹˇĂ‚Â¬Ă„Ä…Ă„Äľ 0 byĂ„â€šĂ˘â‚¬ĹľÄ‚â€žĂ˘â‚¬Â¦Ă„â€šĂ‹ďż˝Ä‚Ë�Ă˘â‚¬ĹˇĂ‚Â¬Ă„Ä…Ă‹â€ˇ w czwartek)

  while( dayno >= YEARSIZE(tm_year) ) // wydobycie aktualnego roku
  {
    dayno -= YEARSIZE(tm_year);
    tm_year++;
  }

  czas->rok = tm_year; // rok

  uint8_t leap, month_len;
  leap = LEAPYEAR(czas->rok);
  month_len = monthlen(leap, tm_mon);

  while( dayno >= month_len ) // wydobycie numeru aktualnego miesiÄ‚â€žĂ˘â‚¬ĹˇÄ‚Ë�Ă˘â€šÂ¬ÄąÄľĂ„â€šĂ‹ďż˝Ä‚Ë�Ă˘â‚¬ĹˇĂ‚Â¬Ä‚â€šĂ‚Â¦ca
  {
    dayno -= month_len;
    tm_mon++;
    month_len = monthlen(leap, tm_mon);
  }

  czas->mies = tm_mon; // miesiÄ‚â€žĂ˘â‚¬ĹˇÄ‚Ë�Ă˘â€šÂ¬ÄąÄľĂ„â€šĂ‹ďż˝Ä‚Ë�Ă˘â‚¬ĹˇĂ‚Â¬Ä‚â€šĂ‚Â¦c

  czas->dzien = dayno; // dzieĂ„â€šĂ˘â‚¬ĹľÄ‚â€žĂ˘â‚¬Â¦Ă„â€šĂ‹ďż˝Ä‚Ë�Ă˘â‚¬ĹˇĂ‚Â¬Ă„Ä…Ă„Äľ

  if( uzyj_latozima ) korekta_czasu_letni_zimowy(czas);

  czas->mies++;
  czas->dzien++;
}
//2018-01-29 16:11
//rrrr-mm-dd gg-nn

void txt_to_czas( const char * buf, const char * pattern, CZAS * czas )
{
  uint16_t wagi[6] = {1000, 10, 10, 10, 10, 10};
  memset(czas, 0, sizeof(CZAS));

  while( *pattern && *buf )
  {
    switch(*pattern)
    {
    case 'R': czas->rok += wagi[0]*( *buf-'0' );
      wagi[0] /= 10;
      break;
    case 'M': czas->mies += wagi[1]*( *buf-'0' );
      wagi[1] /= 10;
      break;
    case 'D': czas->dzien += wagi[2]*( *buf-'0' );
      wagi[2] /= 10;
      break;
    case 'g': czas->godz += wagi[3]*( *buf-'0' );
      wagi[3] /= 10;
      break;
    case 'm': czas->min += wagi[4]*( *buf-'0' );
      wagi[4] /= 10;
      break;
    case 's': czas->sek += wagi[5]*( *buf-'0' );
      wagi[5] /= 10;
      break;
    }

    buf++;
    pattern++;
  }
}

void czas_to_txt( char * buf, const char * pattern, CZAS * czas )
{
  uint16_t wagi[6] = {1000, 10, 10, 10, 10, 10};

  while( *pattern )
  {
    switch(*pattern)
    {
    case 'R': *buf = czas->rok/wagi[0] + '0';
      czas->rok %= wagi[0];
      wagi[0] /= 10;
      break;
    case 'M': *buf = czas->mies/wagi[1] + '0';
      czas->mies %= wagi[1];
      wagi[1] /= 10;
      break;
    case 'D': *buf = czas->dzien/wagi[2] + '0';
      czas->dzien %= wagi[2];
      wagi[2] /= 10;
      break;
    case 'g': *buf = czas->godz/wagi[3] + '0';
      czas->godz %= wagi[3];
      wagi[3] /= 10;
      break;
    case 'm': *buf = czas->min/wagi[4] + '0';
      czas->min %= wagi[4];
      wagi[4] /= 10;
      break;
    case 's': *buf = czas->sek/wagi[5] + '0';
      czas->sek %= wagi[5];
      wagi[5] /= 10;
      break;
    default: *buf = *pattern;
    }

    buf++;
    pattern++;
  }
}






