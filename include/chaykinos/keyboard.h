#ifndef _CHAYKIN_OS_KEYBOARD_H_
#define _CHAYKIN_OS_KEYBOARD_H_ 1

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	uint8_t scancode;
	char keycode;
} char_t;

typedef struct {
	uint8_t exists;
	uint8_t release;
	char keycode;
} kbd_char_t;

void keyboard_init(void);

uint8_t keyboard_getchar(void);
size_t keyboard_gets(char *, size_t);

#ifdef __cplusplus
}
#endif

#endif
