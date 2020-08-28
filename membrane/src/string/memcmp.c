#include <string.h>

int memcmp(const char* s1, const char* s2, size_t n) {
	for (size_t i = 0; i < n; i++) {
		if (s1[i] < s2[i])
			return -1;
		else if (s2[i] < s1[i])
			return 1;
	}
	return 0;
}
