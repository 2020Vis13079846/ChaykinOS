#include <stdlib.h>

int atoi(const char* string) {
	int result = 0, tmp;
	size_t len = strlen(string);
	for (int i = len-1; i >= 0; i--) {
		tmp = string[i] - '0';
		if (tmp >= 0 && tmp < 10) {
			result += tmp * pow(10, len-i-1);
		} else if (i == 0 && string[i] == '-') {
			result *= -1;
		} else {
			return -1;
		}
	}
	return result;
}
