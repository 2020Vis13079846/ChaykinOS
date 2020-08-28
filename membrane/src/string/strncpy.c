#include <string.h>

char* strncpy(char* toHere, const char* fromHere, size_t n) {
	size_t i;
	for (i = 0; i < n && fromHere[i] != '\0'; i++)
		toHere[i] = fromHere[i];
	for (; i < n; i++)
		toHere[i] = '\0';
	return toHere;
}
