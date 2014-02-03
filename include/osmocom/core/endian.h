/*
 * Macros for endian handling that is portable across Linux, BSD
 * and maybe other systems.
 * At the end OSMO_ENDIAN_LITTLE and OSMO_ENDIAN_BIG will be defined
 * and one of the two will be 1.
 */
#pragma once

#if defined(__FreeBSD__)
#include <sys/endian.h>
#else
#include <endian.h>
#endif

#ifndef __BYTE_ORDER
# ifdef __APPLE__
#  define __BYTE_ORDER __DARWIN_BYTE_ORDER
#  define __LITTLE_ENDIAN __DARWIN_LITTLE_ENDIAN
#  define __BIG_ENDIAN __DARWIN_BIG_ENDIAN
# elif defined(__FreeBSD__)
#  define __BYTE_ORDER BYTE_ORDER
#  define __LITTLE_ENDIAN LITTLE_ENDIAN
#  define __BIG_ENDIAN BIG_ENDIAN
# else
#  error "__BYTE_ORDER should be defined by someone"
# endif
#endif


#if __BYTE_ORDER == __LITTLE_ENDIAN
#define OSMO_ENDIAN_LITTLE 1
#define OSMO_ENDIAN_BIG 0
#elif __BYTE_ORDER == __BIG_ENDIAN
#define OSMO_ENDIAN_LITTLE 0
#define OSMO_ENDIAN_BIG 1
#endif

