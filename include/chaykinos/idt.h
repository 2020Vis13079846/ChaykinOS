#ifndef _CHAYKIN_OS_IDT_H_
#define _CHAYKIN_OS_IDT_H_ 1

#include <stdint.h>

#define IDT_NUM_ENTRIES 256

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	uint16_t offset_low;
	uint16_t selector;
	uint8_t zero;
	uint8_t type;
	uint16_t offset_high;
} __attribute__((packed)) idt_entry_t;

typedef struct {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) idt_ptr_t;

extern void idt_flush(idt_ptr_t *);

void idt_init(void);

#ifdef __cplusplus
}
#endif

#endif
