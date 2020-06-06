#ifndef _ASM_SERIAL_H_
#define _ASM_SERIAL_H_ 1

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern uint16_t ports[];

void serial_port_init(uint16_t port);

char serial_read(uint16_t port);
void serial_write(uint16_t port, char ch);

#ifdef __cplusplus
}
#endif

#endif
