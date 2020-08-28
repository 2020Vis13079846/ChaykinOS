#include <stdio.h>

static bool print(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*)data;
	for (size_t i = 0; i < length; i++)
		if (putchar(bytes[i]) == EOF)
			return false;
	return true;
}

int printf(const char* format, ...) {
	va_list args;
	va_start(args, format);
	int written = 0;
	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;
		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			if (maxrem < amount)
				return -1;
			if (!print(format, amount))
				return -1;
			format += amount;
			written += amount;
			continue;
		}
		const char* format_begin = format++;
		if (*format == 'c') {
			format++;
			char c = (char)va_arg(args, int);
			if (!maxrem)
				return -1;
			if (!print(&c, sizeof(c)))
				return -1;
			written++;
		} else if (*format == 's') {
			format++;
			char* str = va_arg(args, char*);
			size_t len = strlen(str);
			if (maxrem < len)
				return -1;
			if (!print(str, len))
				return -1;
			written += len;
		} else if (*format == 'd' || *format == 'i') {
			format++;
			char* buf = 0;
			int number = va_arg(args, int);
			itoa(number, buf, 10);
			size_t len = strlen(buf);
			if (maxrem < len)
				return -1;
			if (!print(buf, len))
				return -1;
			written += len;
		} else if (*format == 'u') {
			format++;
			char* buf = 0;
			uint32_t number = va_arg(args, uint32_t);
			itoa(number, buf, 10);
			size_t len = strlen(buf);
			if (maxrem < len)
				return -1;
			if (!print(buf, len))
				return -1;
			written += len;
		} else if (*format == 'x') {
			format++;
			char* buf = 0;
			int number = va_arg(args, int);
			itoa(number, buf, 16);
			size_t len = strlen(buf);
			if (maxrem < len)
				return -1;
			if (!print(buf, len))
				return -1;
			written += len;
		} else {
			format = format_begin;
			size_t len = strlen(format);
			if (maxrem < len)
				return -1;
			if (!print(format, len))
				return -1;
			written += len;
			format += len;
		}
	}
	va_end(args);
	return written;
}
