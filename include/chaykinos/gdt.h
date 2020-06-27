#ifndef _CHAYKIN_OS_GDT_H_
#define _CHAYKIN_OS_GDT_H_ 1

#include <stddef.h>
#include <stdint.h>

#define GDT_NUM_ENTRIES 3

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) gdt_ptr_t;

typedef struct {
	uint16_t limit;
	uint16_t base_low;
	uint8_t base_middle;
	uint8_t access;
	uint8_t granularity;
	uint8_t base_high;
} __attribute__((packed)) gdt_entry_t;

extern void gdt_flush(gdt_ptr_t *);

void gdt_init(void);

#ifdef __cplusplus
}
#endif

#endif
