#ifndef _CHAYKIN_OS_PIT_H_
#define _CHAYKIN_OS_PIT_H_ 1

#include <chaykinos/interrupts.h>
#include <asm/ports.h>

#define PIT_FREQUENCY 100

#ifdef __cplusplus
extern "C" {
#endif

uint32_t pit_get_ticks(void);
void pit_sleep(uint32_t time);
void pit_init(void);

#ifdef __cplusplus
}
#endif

#endif
