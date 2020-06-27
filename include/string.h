#ifndef _STRING_H_
#define _STRING_H_ 1

#include <sys/cdefs.h>

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

int memcmp(const void *, const void *, size_t);
void* memcpy(void *, const void *, size_t);
void* memmove(void *, const void *, size_t);
void* memset(void *, int, size_t);
size_t strlen(char *);

#ifdef __cplusplus
}
#endif

#endif
