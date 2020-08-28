#include <string.h>

char* strncat(char* dest, const char* src, size_t n) {
	size_t length = strlen(dest), i;
	for (i = 0; i < n && src[i] != '\0'; i++)
		dest[length+i] = src[i];
	dest[length+i] = '\0';
	return dest;
}
