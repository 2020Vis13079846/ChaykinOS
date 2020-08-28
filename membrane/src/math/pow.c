#include <math.h>

int pow(int x, int y) {
	if (y == 0)
		return 1;
	else if (y == 1)
		return x;
	else {
		int result = x;
		for (int i = 1; i < y; i++) {
			result *= x; 
		}
		return result;
	}
	return 1 / pow(-x, y);
}
