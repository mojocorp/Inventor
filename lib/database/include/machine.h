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

#ifndef __MACHINE_H__
#define __MACHINE_H__

/*
 * Copyright (C) 1990,91,92,93   Silicon Graphics, Inc.
 *
 _______________________________________________________________________
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 |
 |   $Revision: 1.5 $
 |
 |   Description:
 |	This include file contains the machine-dependent macros
 |   and defines for converting basic datatypes across machines.
 |   This is only used by the SoInput and SoOutput classes defined
 |   in the Inventor lib/database source.
 |       TO ADD A NEW MACHINE:
 |
 |		1. Add a machine independent setup section
 |		   (define MACHINE_WORD_FORMAT and MACHINE_FLOAT_FORMAT)
 |
 |		2. Define DGL_HTON_*, DGL_NTOH_*, and SHORT/INT32/FLOAT/DOUBLE
 |
 |       IMPORTANT NOTE: The only complete examples found in this
 |   file are the SGI and Cray implementations.  Note that the other
 |   implementations are incomplete!
 |
 |   Author(s)		: Dave Immel
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#if defined(__APPLE__) && (defined(__GNUC__) || defined(__xlC__) || defined(__xlc__))
#  define SB_OS_MACX
#elif defined(__CYGWIN__)
#  define SB_OS_CYGWIN
#elif defined(WIN64) || defined(_WIN64) || defined(__WIN64__)
#  define SB_OS_WIN32
#  define SB_OS_WIN64
#  define SB_OS_WIN
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#  define SB_OS_WIN32
#  define SB_OS_WIN
#elif defined(__sun) || defined(sun)
#  define SB_OS_SOLARIS
#elif defined(__linux__) || defined(__linux)
#  define SB_OS_LINUX
#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__)
#  define SB_OS_BSD
#elif defined(__sgi)
#  define SB_OS_IRIX
#else
#  error "Inventor has not been ported to this OS"
#endif

/*
 * SGI machine dependent setup
 */

#ifdef SB_OS_IRIX

#define MACHINE_WORD_FORMAT	DGL_BIG_ENDIAN
#define MACHINE_FLOAT_FORMAT	DGL_BIG_IEEE
#if m68000			/* on 3K, times() returns 60'ths regardless */
#undef HZ
#define HZ 60
#define double long float    /* System long, 
				make m68020 behave like everyone */
#endif /* m68000 */

#endif /* SB_OS_IRIX */


/*
 * SUN Solaris machine dependent setup
 */

#ifdef SB_OS_SOLARIS

#include <sys/byteorder.h>

#if _BYTE_ORDER == _BIG_ENDIAN
#   define MACHINE_WORD_FORMAT	DGL_BIG_ENDIAN
#   define MACHINE_FLOAT_FORMAT	DGL_BIG_IEEE
#elif _BYTE_ORDER == _LITTLE_ENDIAN
#   define MACHINE_WORD_FORMAT	DGL_LITTLE_ENDIAN
#   define MACHINE_FLOAT_FORMAT	DGL_NON_IEEE
#endif

#endif /* SB_OS_SOLARIS */


/*
 * Windows machine dependent setup
 */

#ifdef SB_OS_WIN

#define MACHINE_WORD_FORMAT     DGL_LITTLE_ENDIAN
#define MACHINE_FLOAT_FORMAT    DGL_NON_IEEE

#endif /* SB_OS_WIN */

/*
 * Linux i386/ia64 machine dependent setup
 */

#ifdef SB_OS_LINUX

#include <endian.h>

#if __BYTE_ORDER == __BIG_ENDIAN
#   define MACHINE_WORD_FORMAT	DGL_BIG_ENDIAN
#   define MACHINE_FLOAT_FORMAT	DGL_BIG_IEEE
#elif __BYTE_ORDER == __LITTLE_ENDIAN
#   define MACHINE_WORD_FORMAT	DGL_LITTLE_ENDIAN
#   define MACHINE_FLOAT_FORMAT	DGL_NON_IEEE
#endif

#endif

/*
 * BSD machine dependent setup
 */

#ifdef SB_OS_BSD

#include <sys/endian.h>

#if _BYTE_ORDER == _BIG_ENDIAN
#   define MACHINE_WORD_FORMAT	DGL_BIG_ENDIAN
#   define MACHINE_FLOAT_FORMAT	DGL_BIG_IEEE
#elif _BYTE_ORDER == _LITTLE_ENDIAN
#   define MACHINE_WORD_FORMAT	DGL_LITTLE_ENDIAN
#   define MACHINE_FLOAT_FORMAT	DGL_NON_IEEE
#endif

#endif

/*
 * Apple Darwin (Mac OS X) machine dependent setup
 */

#ifdef SB_OS_MACX

#ifdef __LITTLE_ENDIAN__
#   define MACHINE_WORD_FORMAT  DGL_LITTLE_ENDIAN
#   define MACHINE_FLOAT_FORMAT DGL_NON_IEEE
#else
#   define MACHINE_WORD_FORMAT  DGL_BIG_ENDIAN
#   define MACHINE_FLOAT_FORMAT DGL_BIG_IEEE
#endif

#endif

#if !defined(MACHINE_WORD_FORMAT) || !defined(MACHINE_FLOAT_FORMAT)
#  error "Inventor needs to be set up for your CPU type."
#endif

/*
 * Defines for the various data formats
 */

#define DGL_LITTLE_ENDIAN 1		/* integer formats		*/
#define DGL_BIG_ENDIAN 2

#define DGL_BIG_IEEE 1			/* floating point formats	*/
#define DGL_NON_IEEE 3


/*
 * Data conversion (byte swapping) algorithms:
 *	HTON - client host to network (server)
 *	NTOH - network (server) to client host
 */


/*
 * DGL_BIG_ENDIAN: no conversion necessary (INTEGER)
 */

#if MACHINE_WORD_FORMAT == DGL_BIG_ENDIAN
#define DGL_HTON_SHORT(t,f) t = f
#define DGL_NTOH_SHORT DGL_HTON_SHORT
#define DGL_HTON_INT32(t,f) t = f
#define DGL_NTOH_INT32 DGL_HTON_INT32
#endif /* MACHINE_WORD_FORMAT */

/*
 * DGL_BIG_IEEE: no conversion necessary (FLOAT)
 */

#if MACHINE_FLOAT_FORMAT == DGL_BIG_IEEE
#define DGL_HTON_FLOAT(t,f) t = f
#define DGL_NTOH_FLOAT DGL_HTON_FLOAT
#define DGL_HTON_DOUBLE(t,f) t = f
#define DGL_NTOH_DOUBLE DGL_HTON_DOUBLE
#endif


/*
 * DGL_LITTLE_ENDIAN: conversion necessary (INTEGER)
 *	NOTE: non-floating point conversions are the same for both
 *		directions and thus one macro suffices
 */

#if MACHINE_WORD_FORMAT == DGL_LITTLE_ENDIAN

/* like DGL_HTON_INT32, but more efficient if f is a constant */
#define DGL_HTON_SHORT(t,f) 	\
	{			\
		short _from = f,_to;	\
		((char *)&_to)[0] = ((char *)&_from)[1];	\
		((char *)&_to)[1] = ((char *)&_from)[0];	\
		t = _to;	\
	}
#define DGL_NTOH_SHORT DGL_HTON_SHORT
#define DGL_HTON_INT32(t,f)	\
	{			\
		int32_t _from = f,_to;	\
		((char *)&_to)[0] = ((char *)&_from)[3];	\
		((char *)&_to)[1] = ((char *)&_from)[2];	\
		((char *)&_to)[2] = ((char *)&_from)[1];	\
		((char *)&_to)[3] = ((char *)&_from)[0];	\
		t = _to;	\
	}
#define DGL_NTOH_INT32 DGL_HTON_INT32

#endif /* LITTLE_ENDIAN */


/*
 * DGL_NON_IEEE: conversion necessary (FLOAT)
 *	conversion is done within procedure calls for simplicity
 */

#if MACHINE_FLOAT_FORMAT == DGL_NON_IEEE
void mem_hton_float(float *t, float *f);
void mem_ntoh_float(float *t, float *f);
void mem_hton_double(double *t, double *f);
void mem_ntoh_double(double *t, double *f);
#define DGL_HTON_FLOAT(t,f) mem_hton_float(&t,&f)
#define DGL_NTOH_FLOAT(t,f) mem_ntoh_float(&t,&f)
#define DGL_HTON_DOUBLE(t,f) mem_hton_double(&t,&f)
#define DGL_NTOH_DOUBLE(t,f) mem_ntoh_double(&t,&f)
#endif


/*
 * get/set a data item located at address p regardless what it really is
 */

#define INT32(p) (*(int32_t *)(p))
#define FLOAT(p) (*(float *)(p))
#define DOUBLE(p) (*(double *)(p))
#define SHORT(p) (*(short *)(p))

#endif /* __MACHINE_H__ */
