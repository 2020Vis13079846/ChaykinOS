#ifndef _CHAYKIN_OS_SSP_H_
#define _CHAYKIN_OS_SSP_H_ 1

#include <stdint.h>

#if UINT32_MAX == UINTPTR_MAX
#  define STACK_CHK_GUARD 0xe2dee396
#else
#  define STACK_CHK_GUARD 0x595e9fbd94fda766
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern uintptr_t __stack_chk_guard;

void __stack_chk_fail(void);

#ifdef __cplusplus
}
#endif

#endif
