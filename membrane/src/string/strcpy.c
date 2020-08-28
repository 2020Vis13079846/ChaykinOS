#include <string.h>

char* strcpy(char* toHere, const char* fromHere) {
	return strncpy(toHere, fromHere, strlen(toHere)+strlen(fromHere));
}
