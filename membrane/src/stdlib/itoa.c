#include <stdlib.h>

void itoa(int number, char* buffer, int radix) {
	char* abc = "abcdefghijklmnopqrstuvwxyz";
	int tmp, i = 0, sign = number >= 0;
	number = abs(number);
	do {
		buffer[i++] = (tmp = number % radix) < 10?tmp + '0':abc[tmp-10];
	} while ((number /= radix) > 0);
	if (!sign)
		buffer[i++] = '-';
	buffer[i] = '\0';
	strrev(buffer);
}
