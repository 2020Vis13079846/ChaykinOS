#ifndef _ASM_SERIAL_H_
#define _ASM_SERIAL_H_ 1

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern uint16_t ports[];

void serial_port_init(uint16_t);

char serial_read(uint16_t);
void serial_write(uint16_t, char);

#ifdef __cplusplus
}
#endif

#endif
