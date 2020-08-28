#include <stdlib.h>
#include <stdio.h>

void abort(void) {
#ifdef __is_libk
	printf("kernel: panic: abort()\n");
#else
	printf("abort()\n");
#endif
	while(1);
}
