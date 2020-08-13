#include <chaykinos/tty.h>
#include <chaykinos/vga.h>
#include <asm/ports.h>
#include <string.h>

volatile size_t tty_row = 0;
volatile size_t tty_column = 0;
uint8_t tty_color;
uint16_t *tty_buffer;

void tty_init(void) {
	tty_color = vga_entry_color(VGA_LIGHT_GREY, VGA_BLACK);
	tty_buffer = VGA_MEMORY;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			size_t index = y * VGA_WIDTH + x;
			tty_buffer[index] = vga_entry(' ', tty_color);
		}
	}
	memset(tty_buffer, 0, VGA_WIDTH * VGA_HEIGHT);
	tty_enable_cursor(14, 15);
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
	memset(tty_buffer, 0, VGA_WIDTH * VGA_HEIGHT);
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
	if (tty_buffer[index+1] != 0) {
		for (size_t i = VGA_WIDTH * VGA_HEIGHT; i > index; i--) {
			tty_buffer[i] = tty_buffer[i-1];
		}
	}
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
			tty_column -= 2;
		}
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
	tty_update_cursor(tty_column, tty_row);
}

void tty_write(char *data, size_t length) {
	for (size_t i = 0; i < length; i++)
		tty_putchar(data[i]);
}

void tty_writestring(char *data) {
	tty_write(data, strlen(data));
}

void tty_putuint(uint32_t num) {
	unsigned int n, d = 1000000000, index = 0;
	char *str = 0;
	while ((num/d == 0) && (d >= 10))
		d /= 10;
	n = num;
	while (d >= 10) {
		str[index++] = ((char)((int)'0' + n/d));
		n = n % d;
		d /= 10;
	}
	str[index++] = ((char)((int)'0' + n));
	str[index] = 0;
	tty_writestring(str);
}

void tty_putint(int32_t num) {
	if (num >= 0) {
		tty_putuint(num);
	} else {
		tty_putchar('-');
		tty_putuint(-num);
	}
}

void tty_puthex(uint32_t num) {
	char *hex = "0123456789abcdef";
	unsigned int n, d = 0x10000000, index = 0;
	char *str = 0;
	while ((num/d == 0) && (d >= 0x10))
		d /= 0x10;
	n = num;
	while (d >= 0xf) {
		str[index++] = hex[n/d];
		n = n % d;
		d /= 0x10;
	}
	str[index++] = hex[n];
	str[index] = 0;
	tty_writestring(str);
}

void tty_putoct(uint32_t num) {
	unsigned int n, d = 2097152, index = 0;
	char *str = 0;
	while ((num/d == 0) && (d >= 10))
		d /= 8;
	n = num;
	while (d >= 8) {
		str[index++] = ((char)((int)'0' + n/d));
		n = n % d;
		d /= 8;
	}
	str[index++] = ((char)((int)'0' + n));
	str[index] = 0;
	tty_writestring(str);
}

void tty_vprintf(const char *format, va_list args) {
	int i = 0;
	while (format[i]) {
		if (format[i] == '%') {
			i++;
			switch (format[i]) {
				case 'c':
					tty_putchar(va_arg(args, int));
					break;
				case 's':
					tty_writestring(va_arg(args, char*));
					break;
				case 'd':
				case 'i':
					tty_putint(va_arg(args, int));
					break;
				case 'u':
					tty_putuint(va_arg(args, uint32_t));
					break;
				case 'x':
					tty_puthex(va_arg(args, uint32_t));
					break;
				case 'o':
					tty_putoct(va_arg(args, uint32_t));
					break;
				default:
					tty_putchar(format[i]);
			}
		} else {
			tty_putchar(format[i]);
		}
		i++;
	}
}

void tty_printf(const char *format, ...) {
	va_list args;
	va_start(args, format);
	tty_vprintf(format, args);
	va_end(args);
}

void tty_enable_cursor(uint8_t start, uint8_t end) {
	outb(0x3d4, 0x0a);
	outb(0x3d5, (inb(0x3d5) & 0xc0) | start);
	outb(0x3d4, 0x0b);
	outb(0x3d5, (inb(0x3d5) & 0xe0) | end);
}

void tty_disable_cursor(void) {
	outb(0x3d4, 0x0a);
	outb(0x3d5, 0x20);
}

void tty_update_cursor(int x, int y) {
	uint16_t pos = y * VGA_WIDTH + x;
	outb(0x3d4, 0x0f);
	outb(0x3d5, (uint8_t)(pos & 0xff));
	outb(0x3d4, 0x0e);
	outb(0x3d5, (uint8_t)((pos >> 8) & 0xff));
}

uint16_t tty_get_cursor_position(void) {
	uint16_t pos = 0;
	outb(0x3d4, 0x0f);
	pos |= inb(0x3d5);
	outb(0x3d4, 0x0e);
	pos |= ((uint16_t)inb(0x3d5)) << 8;
	return pos;
}
