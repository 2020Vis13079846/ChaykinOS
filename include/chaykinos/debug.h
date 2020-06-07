#ifndef _CHAYKIN_OS_DEBUG_H_
#define _CHAYKIN_OS_DEBUG_H_ 1

#include <stdint.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

void debug_putchar(char);
void debug_writestring(char*);
void debug_vprintf(const char*, va_list);
void debug_printf(const char*, ...);

#ifdef __cplusplus
}
#endif

#endif
