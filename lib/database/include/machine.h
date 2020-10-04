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
 |
 |   Author(s)		: Dave Immel
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#if defined(__APPLE__)
#define SB_OS_MACX
#elif defined(WIN64) || defined(_WIN64) || defined(__WIN64__)
#define SB_OS_WIN32
#define SB_OS_WIN64
#define SB_OS_WIN
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define SB_OS_WIN32
#define SB_OS_WIN
#elif defined(__linux__) || defined(__linux)
#define SB_OS_LINUX
#else
#error "Inventor has not been ported to this OS"
#endif

/*
 * Windows machine dependent setup
 */

#ifdef SB_OS_WIN

#define SB_BYTE_ORDER SB_LITTLE_ENDIAN

#endif /* SB_OS_WIN */

/*
 * Linux i386/ia64 machine dependent setup
 */

#ifdef SB_OS_LINUX

#include <endian.h>

#if __BYTE_ORDER == __BIG_ENDIAN
#define SB_BYTE_ORDER SB_BIG_ENDIAN
#elif __BYTE_ORDER == __LITTLE_ENDIAN
#define SB_BYTE_ORDER SB_LITTLE_ENDIAN
#endif

#endif /* SB_OS_LINUX */

/*
 * Apple Darwin (Mac OS X) machine dependent setup
 */

#ifdef SB_OS_MACX

#ifdef __LITTLE_ENDIAN__
#define SB_BYTE_ORDER SB_LITTLE_ENDIAN
#else /* __LITTLE_ENDIAN */
#define SB_BYTE_ORDER SB_BIG_ENDIAN
#endif /* __LITTLE_ENDIAN */

#endif /* SB_OS_MACX */

/*
 * Platform sanity check.
 */

#if !defined(SB_BYTE_ORDER)
#error "Byte order needs to be set up for your CPU type."
#endif

/*
 * Defines for the various data formats
 */

#define SB_LITTLE_ENDIAN 1
#define SB_BIG_ENDIAN 2

/*
 * 32/64-bit architecture dependent statements
 */

/*
 * Data conversion (byte swapping) algorithms:
 *	HTON - client host to network (server)
 *	NTOH - network (server) to client host
 */

/*
 * SB_BIG_ENDIAN: no conversion necessary
 */

#if SB_BYTE_ORDER == SB_BIG_ENDIAN
#define DGL_HTON_SHORT(t, f) t = f
#define DGL_NTOH_SHORT DGL_HTON_SHORT
#define DGL_HTON_INT32(t, f) t = f
#define DGL_NTOH_INT32 DGL_HTON_INT32

#define DGL_HTON_FLOAT(t, f) t = f
#define DGL_NTOH_FLOAT DGL_HTON_FLOAT
#define DGL_HTON_DOUBLE(t, f) t = f
#define DGL_NTOH_DOUBLE DGL_HTON_DOUBLE
#endif

/*
 * SB_LITTLE_ENDIAN: conversion necessary
 */

#if SB_BYTE_ORDER == SB_LITTLE_ENDIAN

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#define swap16(x) _byteswap_ushort(x)
#define swap32(x) _byteswap_ulong(x)
#define swap64(x) _byteswap_uint64(x)
#elif defined(__GNUC__) &&                                                     \
    (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 2))
#define swap16(x) (((x)&0xFF00) >> 8 | ((x)&0x00FF) << 8)
#define swap32(x) __builtin_bswap32(x)
#define swap64(x) __builtin_bswap64(x)
#else
#define swap16(x) (((x)&0xFF00) >> 8 | ((x)&0x00FF) << 8)
#define swap32(x)                                                              \
    ((((x)&0xff000000) >> 24) | (((x)&0x00ff0000) >> 8) |                      \
     (((x)&0x0000ff00) << 8) | (((x)&0x000000ff) << 24))
#define swap64(x)                                                              \
    (((x) >> 56) | (((x) >> 40) & 0xff00) | (((x) >> 24) & 0xff0000) |         \
     (((x) >> 8) & 0xff000000) | (((x) << 8) & ((uint64_t)0xff << 32)) |       \
     (((x) << 24) & ((uint64_t)0xff << 40)) |                                  \
     (((x) << 40) & ((uint64_t)0xff << 48)) | (((x) << 56)))
#endif
/* like DGL_HTON_INT32, but more efficient if f is a constant */
#define DGL_HTON_SHORT(t, f) (t = swap16(f))
#define DGL_HTON_INT32(t, f) (t = swap32(f))
#define DGL_HTON_FLOAT(t, f) (*(int32_t *)&t = swap32(*(int32_t *)&f))
#define DGL_HTON_DOUBLE(t, f) (*(int64_t *)&t = swap64(*(int64_t *)&f))

#define DGL_NTOH_SHORT DGL_HTON_SHORT
#define DGL_NTOH_INT32 DGL_HTON_INT32
#define DGL_NTOH_FLOAT DGL_HTON_FLOAT
#define DGL_NTOH_DOUBLE DGL_HTON_DOUBLE
#endif

/*
 * get/set a data item located at address p regardless what it really is
 */

#define INT32(p) (*(int32_t *)(p))
#define FLOAT(p) (*(float *)(p))
#define DOUBLE(p) (*(double *)(p))
#define SHORT(p) (*(short *)(p))

#endif /* __MACHINE_H__ */
