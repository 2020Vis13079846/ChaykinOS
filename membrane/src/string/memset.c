#include <string.h>

void* memset(void* buf, int z, size_t n) {
	uint8_t* buffer = (uint8_t*)buf;
	for (size_t i = 0; i < n; i++)
		buffer[i] = (uint8_t)z;
	return buf;
}
