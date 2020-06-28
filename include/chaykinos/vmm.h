#ifndef _CHAYKIN_OS_VMM_H_
#define _CHAYKIN_OS_VMM_H_ 1

#include <stdint.h>

#define TEMP_PAGE_ADDR 0xC03FF000

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t kernel_virt_start;
extern uint32_t kernel_virt_end;

typedef struct __attribute__((aligned(4096))) {
	uint32_t pages[1024];
} page_directory_t;

typedef struct __attribute__((aligned(4096))) {
	uint32_t pages[1024];
} page_table_t;

extern void enable_paging(uint32_t);

void vmm_init(void);

void vmm_kernel_directory(void);

int vmm_alloc_page(uint32_t);
void vmm_page_free(uint32_t);
void vmm_page_map(uint32_t, uint32_t);
uint32_t vmm_temp_page_map(uint32_t);

#ifdef __cplusplus
}
#endif

#endif
