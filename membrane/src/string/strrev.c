#include <string.h>

char* strrev(char* str) {
	char tmp = 0;
	for (int start = 0, end = strlen(str)-1; start < end; start++, end--) {
		tmp = str[start];
		str[start] = str[end];
		str[end] = tmp;
	}
	return str;
}
