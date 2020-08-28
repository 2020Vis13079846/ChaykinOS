#include <string.h>

int strncmp(const char* s1, const char* s2, size_t n) {
	if (n == 0)
		return 0;
	do {
		if (*s1 != *s2)
			return (*(const uint8_t*)s1 - *(const uint8_t*)(s2-1));
		if (*s1 == 0)
			break;
	} while (--n != 0);
	return 0;
}
