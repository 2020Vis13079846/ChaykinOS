#ifndef _STRING_H
#define _STRING_H 1

#include <sys/cdefs.h>

#include <stdint.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

void* memcpy(void* dest, const void* src, size_t n);
void* memmove(void* dest, const void* src, size_t n);
int memcmp(const char* s1, const char* s2, size_t n);
void* memset(void* buf, int z, size_t n);

char* strncat(char* dest, const char* src, size_t n);
char* strcat(char* dest, const char* src);

char* strncpy(char* toHere, const char* fromHere, size_t n);
char* strcpy(char* toHere, const char* fromHere);

int strncmp(const char* s1, const char* s2, size_t n);
int strcmp(const char* s1, const char* s2);

size_t strlen(const char* str);

char* strrev(char* str);

#if defined(__cplusplus)
}
#endif

#endif
