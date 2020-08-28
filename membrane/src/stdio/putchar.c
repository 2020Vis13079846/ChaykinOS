#include <stdio.h>

#ifdef __is_libk
#include <kernel/tty.h>
#endif

int putchar(int character) {
#ifdef __is_libk
	char c = (char)character;
	tty_write(&c, sizeof(c));
#else
	// TODO: System call.
#endif
	return character;
}
