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

//  -*- C++ -*-

/*
 * Copyright (C) 1990,91   Silicon Graphics, Inc.
 *
 _______________________________________________________________________
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 |
 |   $Revision: 1.3 $
 |
 |   Description:
 |	This file defines the SbTime class for manipulating times
 |
 |   Classes:
 |	SbTime
 |
 |   Author(s)		: Nick Thompson
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef _SB_TIME_
#define _SB_TIME_

#include <time.h>

#include <Inventor/SbBasic.h>
#include <Inventor/SbString.h>

/// Class for representation of a time.
/// \ingroup Basics
/// This class represents and performs operations on time. Operations may be
/// done in seconds, seconds and microseconds, or using a
/// <b>struct timeval</b>
/// (defined in <em>/usr/include/sys/time.h</em>).
/// \sa cftime
class SbTime {
  public:
    /// Default constructor
    SbTime() {}

    /// Constructor taking a double (in seconds)
    SbTime(double sec);

    /// Constructor taking seconds + microseconds
    SbTime(time_t sec, long usec); // System long from <sys/time.h>

  private:
    // Constructor taking milliseconds
    //
    // NOTE! This constructor has been removed.  Change existing uses of
    // 		SbTime(msec)
    // to
    //		time_t secs = msec / 1000;
    //		SbTime(secs, 1000 * (msec - 1000 * sec))
    // The constructor was removed because it led to unexpected results --
    // while SbTime(1.0) results in 1 second, SbTime(1) resulted in 1
    // MILLIsecond).  Its declaration has been kept, as "private", so that
    // existing code using it will get compilation errors; if it was removed
    // completely, an existing use of SbTime(1) would silently cast to
    // SbTime(1.0) resulting in hard-to-find bugs.  This declaration
    // will be removed entirely in a future release, so that SbTime(1)
    // will be equivalent to SbTime(1.0).
    SbTime(uint32_t msec);

  public:
    /// Constructor taking struct timeval
    SbTime(const struct timeval *tv);

    /// Get the current time (seconds since Jan 1, 1970)
    static SbTime getTimeOfDay();

    /// Set to the current time (seconds since Jan 1, 1970)
    void setToTimeOfDay();

    /// Get a zero time
    static SbTime zero() { return SbTime(0, 0); }

    /// Get a time far, far into the future
    static SbTime maxTime();

    /// Set time from a double (in seconds)
    void setValue(double sec);

    /// Set time from seconds + microseconds
    void setValue(time_t sec, long usec); // System long

    /// Set time from a struct timeval
    void setValue(const struct timeval *tv);

    /// Set time from milliseconds
    void setMsecValue(unsigned long msec); // System long

    /// Get time in seconds as a double
    double getValue() const;

    /// Get time in seconds & microseconds
    void getValue(time_t &sec, long &usec) const; // System long

    /// Get time in a struct timeval
    void getValue(struct timeval *tv) const;

    /// Get time in milliseconds (for Xt)
    unsigned long getMsecValue() const; // System long

    /// Convert to a string.  The default format is seconds with
    /// 3 digits of fraction precision.  \a fmt is a character string that
    /// consists of field descriptors and text characters, in a manner analogous
    /// to cftime (3C) and printf (3S). Each field descriptor consists of
    /// a % character followed by another character which specifies the
    /// replacement for the field descriptor.  All other characters are copied
    /// from \a fmt into the result.  The following field descriptors are
    /// supported:
    ///
    /// %   the `%' character
    /// D   total number of days
    /// H   total number of hours
    /// M   total number of minutes
    /// S   total number of seconds
    /// I   total number of milliseconds
    /// U   total number of microseconds
    /// h   hours remaining after the days (00-23)
    /// m   minutes remaining after the hours (00-59)
    /// s   seconds remaining after the minutes (00-59)
    /// i   milliseconds remaining after the seconds (000-999)
    /// u   microseconds remaining after the seconds (000000-999999)
    ///
    /// The uppercase descriptors are formatted with a leading `\(em' for
    /// negative times; the lowercase descriptors are formatted fixed width,
    /// with leading zeros.  For example, a reasonable format string might be
    /// "elapsed\ time:\ %M\ minutes,\ %s\ seconds". The default value of \a
    /// fmt,
    /// "%S.%i", formats the time as seconds with 3 digits of fractional
    /// precision.
    SbString format(const char *fmt = "%S.%i") const;

    /// Convert to a date string, interpreting the time as seconds since
    /// Jan 1, 1970.  The default format gives "Tuesday, 01/26/93 11:23:41 AM".
    /// See the 'cftime()' man page for explanation of the format string.
    SbString formatDate(const char *fmt = "%A, %D %r") const;

    /// Addition of two times.
    friend SbTime operator+(const SbTime &t0, const SbTime &t1);

    /// Subtraction of two times.
    friend SbTime operator-(const SbTime &t0, const SbTime &t1);

    /// Destructive addition
    SbTime &operator+=(const SbTime &tm) { return (*this = *this + tm); }

    /// Destructive subtraction
    SbTime &operator-=(const SbTime &tm) { return (*this = *this - tm); }

    /// Unary negation
    SbTime operator-() const;

    /// multiplication by scalar
    friend SbTime operator*(const SbTime &tm, double s);

    friend SbTime operator*(double s, const SbTime &tm) { return tm * s; }

    /// destructive multiplication by scalar
    SbTime &operator*=(double s) {
        *this = *this * s;
        return *this;
    }

    /// division by scalar
    friend SbTime operator/(const SbTime &tm, double s);

    /// destructive division by scalar
    SbTime &operator/=(double s) { return (*this = *this / s); }

    /// division by another time
    double operator/(const SbTime &tm) const {
        return getValue() / tm.getValue();
    }

    /// modulus for two times
    SbTime operator%(const SbTime &tm) const;

    /// equality operators
    int operator==(const SbTime &tm) const;

    int operator!=(const SbTime &tm) const;

    /// relational operators
    SbBool operator<(const SbTime &tm) const;
    SbBool operator>(const SbTime &tm) const;
    SbBool operator<=(const SbTime &tm) const;
    SbBool operator>=(const SbTime &tm) const;

  private:
    long tv_sec;
    long tv_usec;
};

#endif /* _SB_TIME_ */
