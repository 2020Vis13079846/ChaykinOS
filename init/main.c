/*

	Welcome to ChaykinOS source code!
	ChaykinOS is an OS from scratch.
	It's developed by Simon Chaykin
	(game-lover).

*/

#include <chaykinos/tty.h>

void start_kernel(void) {
	tty_init();
	tty_printf("TTY Initialized.\n");
}

void main(void) {
	tty_printf("Hello, world!\n");
}
