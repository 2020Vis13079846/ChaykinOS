#include <chaykinos/debug.h>
#include <chaykinos/pmm.h>
#include <asm/serial.h>
#include <string.h>

void debug_putchar(char c) {
	serial_write(0x3f8, c);
}

void debug_write(char *data, size_t length) {
	for (size_t i = 0; i < length; i++)
		debug_putchar(data[i]);
}

void debug_writestring(char *data) {
	debug_write(data, strlen(data));
}

void debug_putuint(uint32_t num) {
	unsigned int n, d = 1000000000, index = 0;
	char *str = (char*)pmm_block_alloc();
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
	debug_writestring(str);
	pmm_block_free((uint32_t)str);
}

void debug_putint(int32_t num) {
	if (num >= 0) {
		debug_putuint(num);
	} else {
		debug_putchar('-');
		debug_putuint(-num);
	}
}

void debug_puthex(uint32_t num) {
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
	debug_writestring(str);
}

void debug_putoct(uint32_t num) {
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
	debug_writestring(str);
}

void debug_vprintf(const char *format, va_list args) {
	int i = 0;
	while (format[i]) {
		if (format[i] == '%') {
			i++;
			switch (format[i]) {
				case 'c':
					debug_putchar(va_arg(args, int));
					break;
				case 's':
					debug_writestring(va_arg(args, char*));
					break;
				case 'd':
				case 'i':
					debug_putint(va_arg(args, int));
					break;
				case 'u':
					debug_putuint(va_arg(args, uint32_t));
					break;
				case 'x':
					debug_puthex(va_arg(args, uint32_t));
					break;
				case 'o':
					debug_putoct(va_arg(args, uint32_t));
					break;
				default:
					debug_putchar(format[i]);
			}
		} else {
			debug_putchar(format[i]);
		}
		i++;
	}
}

void debug_printf(const char *format, ...) {
	va_list args;
	va_start(args, format);
	debug_vprintf(format, args);
	va_end(args);
}
