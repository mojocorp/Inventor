/*
 *
 *  Copyright (C) 2000 Silicon Graphics, Inc.  All Rights Reserved. 
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  Further, this software is distributed without any warranty that it is
 *  free of the rightful claim of any third person regarding infringement
 *  or the like.  Any license provided herein, whether implied or
 *  otherwise, applies only to this software file.  Patent licenses, if
 *  any, provided herein do not apply to combinations of this program with
 *  other software, or any other product whatsoever.
 * 
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Contact information: Silicon Graphics, Inc., 1600 Amphitheatre Pkwy,
 *  Mountain View, CA  94043, or:
 * 
 *  http://www.sgi.com 
 * 
 *  For further information regarding this notice, see: 
 * 
 *  http://oss.sgi.com/projects/GenInfo/NoticeExplan/
 *
 */

/*
 * Copyright (C) 1990,91   Silicon Graphics, Inc.
 *
 _______________________________________________________________________
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 |
 |   $Revision: 1.6 $
 |
 |   Classes:
 |	SbTime
 |
 |   Author(s)		: Nick Thompson
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <Inventor/SbTime.h>
#include <time.h>
#include <math.h>
#include <limits>

#ifdef SB_OS_WIN
#   include <winsock.h>
#   include <sys/timeb.h>
#else
#   include <sys/time.h>
#endif

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Constructor that takes the number of seconds as a double
//
// Use: public

SbTime::SbTime(double sec)
//
////////////////////////////////////////////////////////////////////////
{
    setValue(sec);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
// Constructor taking seconds + microseconds
//

SbTime::SbTime(time_t sec, long usec)
{ 
    setValue(sec, usec);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
// Constructor taking struct timeval
//
SbTime::SbTime(const struct timeval *tv)
{ 
    setValue(tv);
}
////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns the current time in an SbTime.
//
// Use: public

SbTime
SbTime::getTimeOfDay()
//
////////////////////////////////////////////////////////////////////////
{
    SbTime	tm;
#ifdef SB_OS_WIN
    struct _timeb timeBuffer;
    _ftime(&timeBuffer);
    tm.tv_sec = (long)timeBuffer.time;
    tm.tv_usec = (long)(timeBuffer.millitm * 1000.0);
#else
    struct timeval t;
    if (-1 == gettimeofday(&t, NULL))
    perror("gettimeofday");
    tm.tv_sec = t.tv_sec;
    tm.tv_usec = t.tv_usec;
#endif

    return tm;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sets to the current time.
//
// Use: public

void
SbTime::setToTimeOfDay()
//
////////////////////////////////////////////////////////////////////////
{
    *this = SbTime::getTimeOfDay();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
// Get a time far, far into the future
SbTime SbTime::maxTime()
{ 
    return SbTime(std::numeric_limits<time_t>::max(), 999999); 
}

// Set time from a double (in seconds)
void 
SbTime::setValue(double sec)
{ 
    tv_sec = time_t(int(sec)); 
    tv_usec = long((sec - tv_sec) * 1000000.0); 
}

// Set time from seconds + microseconds
void
SbTime::setValue(time_t sec, long usec)  	// System long
{ 
    tv_sec = (long)sec;
    tv_usec = usec;
}

// Set time from a struct timeval
void
SbTime::setValue(const struct timeval *tv)
{ 
    tv_sec = tv->tv_sec; 
    tv_usec = tv->tv_usec; 
}

// Set time from milliseconds
void
SbTime::setMsecValue(unsigned long msec)  	// System long
{ 
    tv_sec = time_t(msec/1000); 
    tv_usec = long(1000 * (msec % 1000)); 
}

// Get time in seconds as a double
double
SbTime::getValue() const
{ 
    return (double) tv_sec + (double) tv_usec / 1000000.0; 
}

// Get time in seconds & microseconds
void
SbTime::getValue(time_t &sec, long &usec) const  // System long
{ 
    sec = tv_sec; 
    usec = tv_usec; 
}

// Get time in a struct timeval
void
SbTime::getValue(struct timeval *tv) const
{ 
    tv->tv_sec = tv_sec; 
    tv->tv_usec = tv_usec; 
}

// Get time in milliseconds (for Xt)
unsigned long
SbTime::getMsecValue() const			// System long
{ 
    return tv_sec * 1000 + tv_usec / 1000; 
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Converts to a formatted string.  The format string supports the following:
//
//	%%			the '%' character
//	%D	 		total number of days
//    	%H	 		total number of hours 
//	%M			total number of minutes
//    	%S			total number of seconds
//	%I			total number of milliseconds
//	%U			total number of microseconds
//	%h	00-23		hours remaining after the days
//	%m	00-59		minutes remaining after the hours
//	%s	00-59		seconds remaining after the minutes
//	%i	000-999		milliseconds remaining after the seconds
//	%u	000000-999999	microseconds remaining after the seconds
//
// uppercase descriptors are formatted with a leading '-' for negative times
// lowercase descriptors are formatted fixed width with leading zeros
//
// Use: public

SbString
SbTime::format(const char *fmt) const
//
////////////////////////////////////////////////////////////////////////
{
    SbBool		negative;
    struct timeval	tv;

    // turn into sign-magnitude form
    if (tv_sec >= 0) {
	negative = 0;
    tv.tv_sec = tv_sec;
    tv.tv_usec = tv_usec;
    }
    else {
	negative = 1;
	tv.tv_sec = (-(*this)).tv_sec;
    tv.tv_usec = (-(*this)).tv_usec;
    }

    // first calculate total durations
    const int32_t tday = tv.tv_sec / (60*60*24);
    const int32_t thour = tv.tv_sec / (60*60);
    const int32_t tmin = tv.tv_sec / 60;
    const int32_t tsec = tv.tv_sec;
    const int32_t tmilli = 1000*tv.tv_sec + tv.tv_usec / 1000;
    const int32_t tmicro = 1000000*tv.tv_sec + tv.tv_usec;

    // then calculate remaining durations
    const int32_t rhour = thour - 24*tday;
    const int32_t rmin = tmin - 60*thour;
    const int32_t rsec = tsec - 60*tmin;
    const int32_t rmilli = tmilli - 1000*tsec;
    const int32_t rmicro = tmicro - 1000000*tsec;
    
    char buf[200];
    char *s = buf;

    for (; *fmt; fmt++) {
	if (*fmt != '%')
	    *s++ = *fmt;
	else
	    switch(*++fmt) {
	      case 0:
		fmt--;	// trailing '%' in format string
		break;

	      case '%':
		*s++ = '%';	// "%%" in format string
		break;

	      case 'D':
		if (negative) *s++ = '-';
                s += sprintf(s, "%d", tday);
		break;

	      case 'H':
		if (negative) *s++ = '-';
                s += sprintf(s, "%d", thour);
		break;

	      case 'M':
		if (negative) *s++ = '-';
                s += sprintf(s, "%d", tmin);
		break;

	      case 'S':
		if (negative) *s++ = '-';
                s += sprintf(s, "%d", tsec);
		break;

	      case 'I':
		if (negative) *s++ = '-';
                s += sprintf(s, "%d", tmilli);
		break;

	      case 'U':
		if (negative) *s++ = '-';
                s += sprintf(s, "%d", tmicro);
		break;

	      case 'h':
                s += sprintf(s, "%.2d", rhour);
		break;

	      case 'm':
                s += sprintf(s, "%.2d", rmin);
		break;

	      case 's':
                s += sprintf(s, "%.2d", rsec);
		break;

	      case 'i':
                s += sprintf(s, "%.3d", rmilli);
		break;

	      case 'u':
                s += sprintf(s, "%.6d", rmicro);
		break;

	      default:
		*s++ = '%';	// echo any bad '%?'
		*s++ = *fmt;	// specifier
	    }
        if (s-buf >= (char)sizeof(buf)-7) // don't overshoot the buffer
	    break;
    }
    *s = 0;

    return buf;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Formats as an absolute date/time, using unix "strftime" mechanism.
//
// Use: public

SbString
SbTime::formatDate(const char *fmt) const
//
////////////////////////////////////////////////////////////////////////
{
    char buf[200];
    time_t tt = tv_sec;
    strftime(buf, sizeof(buf), fmt, localtime(&tt));
    return buf;
}

// Unary negation
SbTime
SbTime::operator -() const
{ 
    return (tv_usec == 0) ? SbTime(- tv_sec, 0) : SbTime(- tv_sec - 1, 1000000 - tv_usec); 
}

// modulus for two times
SbTime
SbTime::operator %(const SbTime &tm) const
{ 
    return *this - tm * floor(*this / tm); 
}

// equality operators
int
SbTime::operator ==(const SbTime &tm) const
{ 
    return (tv_sec == tm.tv_sec) && (tv_usec == tm.tv_usec); 
}

int
SbTime::operator !=(const SbTime &tm) const
{ 
    return ! (*this == tm); 
}
////////////////////////////////////////////////////////////////////////
//
// Description:
//
// Use: public

SbTime
operator +(const SbTime &t0, const SbTime &t1)
//
////////////////////////////////////////////////////////////////////////
{
    SbTime tm(t0.tv_sec + t1.tv_sec,
	      t0.tv_usec + t1.tv_usec);
    if (tm.tv_usec >= 1000000) {
	tm.tv_sec  += 1;
	tm.tv_usec -= 1000000;
    }
    return tm;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//
// Use: public

SbTime
operator -(const SbTime &t0, const SbTime &t1)
//
////////////////////////////////////////////////////////////////////////
{
    int32_t sec; 
    long    usec;					// System long

    sec =  t0.tv_sec - t1.tv_sec;
    usec = t0.tv_usec - t1.tv_usec;

    while (usec < 0 && sec > 0) {
	usec += 1000000;
	sec -= 1;
    }

    return SbTime(sec, usec);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//
// Use: public

SbTime
operator *(const SbTime &tm, double s)
//
////////////////////////////////////////////////////////////////////////
{
    return SbTime(tm.getValue() * s);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//
// Use: public

SbTime
operator /(const SbTime &tm, double s)
//
////////////////////////////////////////////////////////////////////////
{
    return tm * (1.0 / s);
}

SbBool
SbTime::operator <(const SbTime &tm) const
{
    if ((tv_sec < tm.tv_sec) ||
	(tv_sec == tm.tv_sec && tv_usec < tm.tv_usec))
	return TRUE;
    else
	return FALSE;
}

SbBool
SbTime::operator >(const SbTime &tm) const
{
    if ((tv_sec > tm.tv_sec) ||
	(tv_sec == tm.tv_sec && tv_usec > tm.tv_usec))
	return TRUE;
    else
	return FALSE;
}

SbBool
SbTime::operator <=(const SbTime &tm) const
{
    if ((tv_sec < tm.tv_sec) ||
	(tv_sec == tm.tv_sec && tv_usec <= tm.tv_usec))
	return TRUE;
    else
	return FALSE;
}

SbBool
SbTime::operator >=(const SbTime &tm) const
{
    if ((tv_sec > tm.tv_sec) ||
	(tv_sec == tm.tv_sec && tv_usec >= tm.tv_usec))
	return TRUE;
    else
	return FALSE;
}

