#ifndef _STDIO_H
#define _STDIO_H 1

#include <sys/cdefs.h>

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <limits.h>

#define EOF -1

#if defined(__cplusplus)
extern "C" {
#endif

int putchar(int character);
int puts(const char* string);

int printf(const char* format, ...);

#if defined(__cplusplus)
}
#endif

#endif
