#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <sys/cdefs.h>

#include <string.h>
#include <math.h>

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct {
    int quot, rem;
} div_t;

typedef struct {
    long int quot, rem;
} ldiv_t;

int atoi(const char* str);

int rand(void);
void srand(unsigned int seed);

void abort(void);

int abs(int number);

void itoa(int number, char *buffer, int radix);

#if defined(__cplusplus)
}
#endif

#endif
