#include <string.h>

int memcmp(const void *s1, const void *s2, size_t n) {
	size_t i;
	const unsigned char *a = (const unsigned char *) s1;
	const unsigned char *b = (const unsigned char *) s2;
	for (i = 0; i < n; i++) {
		if (a[i] < b[i])
			return -1;
		else if (b[i] < a[i])
			return 1;
	}
	return 0;
}

void *memcpy(void *dest, const void *src, size_t n) {
	size_t i;
	unsigned char *dst = (unsigned char *) dest;
	const unsigned char *source = (const unsigned char *) src;
	for (i = 0; i < n; i++)
		dst[i] = source[i];
	return dest;
}

void *memmove(void *dest, const void *src, size_t n) {
	size_t i;
	unsigned char *dst = (unsigned char *) dest;
	const unsigned char *source = (const unsigned char *) src;
	if (dst < source) {
		for (i = 0; i < n; i++)
			dst[i] = source[i];
	} else {
		for (i = n; i != 0; i--)
			dst[i-1] = source[i-1];
	}
	return dest;
}

void *memset(void *dest, int z, size_t n) {
	size_t i;
	unsigned char *dst = (unsigned char *) dest;
	for (i = 0; i < n; i++)
		dst[i] = (unsigned char) z;
	return dest;
}

size_t strlen(char *str) {
	size_t len = 0;
	while (str[++len]);
	return len;
}
