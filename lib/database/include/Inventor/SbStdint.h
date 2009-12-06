#ifndef _SB_STDINT_
#define _SB_STDINT_

#include <machine.h>

#ifdef SB_OS_WIN
    // Support for standard integer types.  This is only a small part of what
    // stdint.h provides on Unix platforms.
    typedef signed __int8     int8_t;
    typedef signed __int16    int16_t;
    typedef signed __int32    int32_t;
    typedef unsigned __int8   uint8_t;
    typedef unsigned __int16  uint16_t;
    typedef unsigned __int32  uint32_t;
    typedef signed __int64    int64_t;
    typedef unsigned __int64  uint64_t;
#else
#   include <stdint.h>
#endif

#endif // _SB_STDINT_
