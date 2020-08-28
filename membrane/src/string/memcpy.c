#include <string.h>

void* memcpy(void* dest, const void* src, size_t n) {
	uint8_t* destination = (uint8_t*)dest;
	const uint8_t* source = (const uint8_t*)src;
	for (size_t i = 0; i < n; i++)
		destination[i] = source[i];
	return dest;
}
