#include <string.h>

void* memmove(void* dest, const void* src, size_t n) {
	uint8_t* destination = (uint8_t*)dest;
	const uint8_t* source = (const uint8_t*)src;
	if (destination < source) {
		for (size_t i = 0; i < n; i++)
			destination[i] = source[i];
	} else {
		for (size_t i = n; i != 0; i--)
			destination[i-1] = source[i-1];
	}
	return dest;
}
