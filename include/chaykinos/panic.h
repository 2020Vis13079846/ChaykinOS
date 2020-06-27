#ifndef _CHAYKIN_OS_PANIC_H_
#define _CHAYKIN_OS_PANIC_H_ 1

#include <chaykinos/tty.h>

#ifdef __cplusplus
extern "C" {
#endif

void panic(char *, ...);

#ifdef __cplusplus
}
#endif

#endif
