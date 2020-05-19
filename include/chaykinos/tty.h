#ifndef _CHAYKIN_OS_TTY_H_
#define _CHAYKIN_OS_TTY_H_ 1

#include <chaykinos/vga.h>
#include <stdbool.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

void tty_init(void);
void tty_setcolor(uint8_t);
void tty_clear(void);
void tty_putchar(char);
void tty_writestring(char*);
void tty_vprintf(const char*, va_list);
void tty_printf(const char*, ...);

#ifdef __cplusplus
}
#endif

#endif
