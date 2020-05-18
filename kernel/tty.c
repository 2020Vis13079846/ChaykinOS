#include <chaykinos/tty.h>

size_t tty_row = 0;
size_t tty_column = 0;
uint8_t tty_color;
uint16_t* tty_buffer;

size_t strlen(char* str) {
	size_t len = 0;
	while (str[++len]);
	return len;
}

void tty_init(void) {
	tty_color = vga_entry_color(VGA_LIGHT_GREY, VGA_BLACK);
	tty_buffer = VGA_MEMORY;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			size_t index = y * VGA_WIDTH + x;
			tty_buffer[index] = vga_entry(' ', tty_color);
		}
	}
}

void tty_clear(void) {
	tty_row = 0;
	tty_column = 0;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			size_t index = y * VGA_WIDTH + x;
			tty_buffer[index] = vga_entry(' ', tty_color);
		}
	}
}

void tty_scroll(void) {
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			tty_buffer[(y * VGA_WIDTH + x)] = tty_buffer[((y+1) * VGA_WIDTH + x)];
		}
	}
}

void tty_setcolor(uint8_t color) {
	tty_color = color;
}

void tty_putentry(char c, uint8_t color, size_t x, size_t y) {
	size_t index = y * VGA_WIDTH + x;
	tty_buffer[index] = vga_entry(c, color);
}

void tty_putchar(char c) {
	if (c == '\n') {
		tty_column = VGA_WIDTH - 1;
	} else if (c == '\t') {
		tty_column += 3;
	} else if (c == '\b') {
		if (tty_column == 0) {
			if (tty_row > 0) {
				tty_row--;
			}
			tty_column = VGA_WIDTH-2;
		} else {
			tty_column-=2;
		}
		tty_putentry(' ', tty_color, tty_column+1, tty_row);
	} else {
		tty_putentry(c, tty_color, tty_column, tty_row);
	}
	if (++tty_column >= VGA_WIDTH) {
		tty_column = 0;
		if (++tty_row >= VGA_HEIGHT) {
			tty_scroll();
			tty_row--;
		}
	}
}

void tty_write(char* data, size_t length) {
	for (size_t i = 0; i < length; i++)
		tty_putchar(data[i]);
}

void tty_writestring(char* data) {
	tty_write(data, strlen(data));
}
