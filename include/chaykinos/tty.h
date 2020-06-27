#ifndef _CHAYKIN_OS_TTY_H_
#define _CHAYKIN_OS_TTY_H_ 1

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

volatile size_t tty_row, tty_column;

void tty_init(void);
void tty_setcolor(uint8_t);
void tty_clear(void);

void tty_putchar(char);
void tty_writestring(char *);
void tty_vprintf(const char *, va_list);
void tty_printf(const char *, ...);

void tty_enable_cursor(uint8_t, uint8_t);
void tty_disable_cursor(void);
void tty_update_cursor(int, int);
uint16_t tty_get_cursor_position(void);

#ifdef __cplusplus
}
#endif

#endif
