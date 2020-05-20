#ifndef _ASM_PORTS_H_
#define _ASM_PORTS_H_ 1

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint8_t inb(uint16_t);
uint16_t inw(uint16_t);
uint32_t inl(uint16_t);

void outb(uint16_t, uint8_t);
void outw(uint16_t, uint16_t);
void outl(uint16_t, uint32_t);

#ifdef __cplusplus
}
#endif

#endif
