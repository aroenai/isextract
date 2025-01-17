/*
  dostime.h - function prototypes
  Copyright (C) 1999, 2002  Bryan Burns
  
  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef __DOSTIME_H
#define __DOSTIME_H

/* Hide warnings about unreferenced local functions */
#if defined(__clang__)
#	pragma clang diagnostic ignored "-Wunused-function"
#elif defined(_MSC_VER)
#	pragma warning(disable:4505)
#elif defined(__GNUC__)
#	pragma GCC diagnostic ignored "-Wunused-function"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>

static time_t dos2unixtime(unsigned long dostime);
static unsigned long unix2dostime(time_t*);
static void dos2struct(unsigned long dostime, struct tm* ltime);

/*
 * The specification to which this was written.  From Joe Buck.
 * The DOS format appears to have only 2 second resolution.  It is an
 * unsigned long, and ORs together
 * 
 * (year-1980)<<25
 * month<<21  (month is tm_mon + 1, 1=Jan through 12=Dec)
 * day<<16    (day is tm_mday, 1-31)
 * hour<<11   (hour is tm_hour, 0-23)
 * min<<5	   (min is tm_min, 0-59)
 * sec>>1	   (sec is tm_sec, 0-59, that's right, we throw away the LSB)
 * 
 * DOS uses local time, so the localtime() call is used to turn the time_t
 * into a struct tm.
 */

static time_t dos2unixtime (unsigned long dostime)
{
  struct tm ltime;
  time_t now = time (NULL);

  /* Call localtime to initialize timezone in TIME.  */
  ltime = *localtime (&now);

  ltime.tm_year = (dostime >> 25) + 80;
  ltime.tm_mon = ((dostime >> 21) & 0x0f) - 1;
  ltime.tm_mday = (dostime >> 16) & 0x1f;
  ltime.tm_hour = (dostime >> 11) & 0x0f;
  ltime.tm_min = (dostime >> 5) & 0x3f;
  ltime.tm_sec = (dostime & 0x1f) << 1;

  ltime.tm_wday = -1;
  ltime.tm_yday = -1;
  ltime.tm_isdst = -1;

  return mktime (&ltime);
}

static void dos2struct(unsigned long dostime, struct tm* ltime)
{
  time_t now = time (NULL);

  /* Call localtime to initialize timezone in TIME.  */
  *ltime = *localtime (&now);

  ltime->tm_year = (dostime >> 25) + 80;
  ltime->tm_mon = ((dostime >> 21) & 0x0f) - 1;
  ltime->tm_mday = (dostime >> 16) & 0x1f;
  ltime->tm_hour = (dostime >> 11) & 0x0f;
  ltime->tm_min = (dostime >> 5) & 0x3f;
  ltime->tm_sec = (dostime & 0x1f) << 1;

  ltime->tm_wday = -1;
  ltime->tm_yday = -1;
  ltime->tm_isdst = -1;
  
  //return ltime
}

static unsigned long unix2dostime (time_t *time)
{
  struct tm *ltime = localtime (time);
  int year = ltime->tm_year - 80;
  if (year < 0)
    year = 0;

  return (year << 25
	  | (ltime->tm_mon + 1) << 21
	  | ltime->tm_mday << 16
	  | ltime->tm_hour << 11
	  | ltime->tm_min << 5
	  | ltime->tm_sec >> 1);
}

#ifdef __cplusplus
}
#endif

#endif /* __DOSTIME_H */
