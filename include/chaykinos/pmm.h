#ifndef _CHAYKIN_OS_PMM_H_
#define _CHAYKIN_OS_PMM_H_ 1

#include <stdbool.h>
#include <chaykinos/multiboot.h>

#ifdef __cplusplus
extern "C" {
#endif

void pmm_init(multiboot_info_t*);

int pmm_find_free_block(void);
uint32_t pmm_block_alloc(void);
void pmm_block_free(uint32_t);
bool pmm_is_block_alloced(uint32_t);

#ifdef __cplusplus
}
#endif

#endif
