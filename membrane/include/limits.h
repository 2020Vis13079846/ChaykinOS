#ifndef _LIMITS_H
#define _LIMITS_H 1

#include <sys/cdefs.h>

#include <stdint.h>

#define CHAR_MIN 0
#define CHAR_MAX 255

#define CHAR_BIT 8
#define SCHAR_MIN (-128)
#define SCHAR_MAX 127
#define UCHAR_MAX 255
#define SHRT_MIN (-1-0x7fff)
#define SHRT_MAX 0x7fff
#define USHRT_MAX 0xffff
#define INT_MIN (-1-0x7fffffff)
#define INT_MAX 0x7fffffff
#define UINT_MAX 0xffffffffU

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif
