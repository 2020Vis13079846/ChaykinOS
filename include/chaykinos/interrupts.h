#ifndef _CHAYKIN_OS_INTERRUPTS_H_
#define _CHAYKIN_OS_INTERRUPTS_H_ 1

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	uint32_t gs, fs, es, ds;
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t idt_index, err_code;
	uint32_t eip, cs, eflags, useresp, ss;
} registers_t;

typedef void (*interrupt_handler_t)(registers_t *);

bool register_interrupt_handler(int, interrupt_handler_t);
bool unregister_interrupt_handler(int);

#ifdef __cplusplus
}
#endif

#endif
