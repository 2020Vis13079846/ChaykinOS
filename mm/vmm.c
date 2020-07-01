#include <chaykinos/vmm.h>
#include <chaykinos/pmm.h>
#include <chaykinos/panic.h>
#include <chaykinos/interrupts.h>
#include <string.h>

page_directory_t *kernel_directory;

void page_fault(__attribute__((unused)) registers_t *r) {
	uint32_t faulting_address;
	asm volatile("mov %%cr2, %0" : "=r" (faulting_address));
	panic("Page fault at addr 0x%x\n", faulting_address);
}

int vmm_page_alloc(uint32_t virtual_address) {
	uint32_t physical_address = pmm_block_alloc();
 	if (!physical_address) {
	 	return 0;
	}
	vmm_page_map(physical_address, virtual_address);
	return 1;
}

void vmm_page_free(uint32_t virtual_address) {
	uint32_t *table = (uint32_t*)(0xFFC00000 + (virtual_address >> 12) * 4);
	if (!(*table & 1)) {
		return;
	}
	uint32_t block = *table & 0x7FFFF000;
	if (block) {
  		pmm_block_free(block);
  	}
	*table &= ~1;
}

void vmm_kernel_directory(void) {
	kernel_directory = (page_directory_t*)pmm_block_alloc();
	if ((uint32_t)kernel_directory == 0xFFFFFFFF) {
		return;
	}
	page_directory_t *temp = kernel_directory;
	memset(temp, 0, sizeof(page_directory_t));
	for (int i = 0; i < 1024; i++) {
		uint32_t *directory = (uint32_t*)&temp->pages[i];
		*directory = (*directory | 2) & ~1;
		if (i == 1023) {
			*directory = ((*directory | 1) & ~0x7FFFF000) | (uint32_t)kernel_directory;
		}
	}
}

void vmm_page_map(uint32_t physical_address, uint32_t virtual_address) {
	uint32_t *directory = (uint32_t*)(0xFFFFF000 + (virtual_address >> 22) * 4);
	if (!(*directory & 1)) {
		uint32_t physical_table = pmm_block_alloc();
		if (physical_table == 0xFFFFFFFF) {
			return;
		}
		page_table_t *virtual_table = (page_table_t*)vmm_temp_page_map(physical_table);
		memset(virtual_table, 0, sizeof(page_table_t));
		*directory = ((*directory | 3) & ~0x7FFFF000) | physical_table;
	}
	uint32_t *table = (uint32_t *)(0xFFC00000 + (TEMP_PAGE_ADDR >> 12) * 4);
	*table = ((*table & ~0x7FFFF000) | physical_address) | 3;
	asm volatile("invlpg (%0)" : : "b"(virtual_address) : "memory");
}

uint32_t vmm_temp_page_map(uint32_t physical_address) {
	uint32_t *table = (uint32_t*)(0xFFC00000 + (TEMP_PAGE_ADDR >> 12) * 4);
	*table = ((*table & ~0x7FFFF000) | ((physical_address) & -4096)) | 3;
	asm volatile("invlpg %0" :: "m" (*(uint32_t *)TEMP_PAGE_ADDR) : "memory" );
	return TEMP_PAGE_ADDR;
}

void vmm_init(void) {
	vmm_kernel_directory();

	{
		page_table_t *table = (page_table_t*)pmm_block_alloc();
		memset((void*)table, 0, sizeof(page_table_t));
		for (uint32_t frame = 0x0, virt = 0xC0000000; frame < 0x100000; frame += 4096, virt += 4096) {
			uint32_t page = 1;
			page = (page & ~0x7FFFF000) | frame;
			table->pages[(((virt) >> 12) & 0x3FF)] = page;
		}
		uint32_t *directory = (uint32_t*)&kernel_directory->pages[0];
		*directory = ((*directory | 3) & ~0x7FFFF000) | (uint32_t)table;
	}
	{
    	page_table_t* table = (page_table_t*)pmm_block_alloc();
    	memset((void*)table, 0, sizeof(page_table_t));
    	for (uint32_t frame = (uint32_t)&kernel_phys_start, virt = (uint32_t)&kernel_virt_start; frame < kernel_phys_map_end; frame += 4096, virt += 4096) {
        	uint32_t page = 1;
			page = (page & ~0x7FFFF000) | frame;
        	table->pages[(((virt) >> 12) & 0x3FF)] = page;
    	}
		uint32_t *directory = (uint32_t*)&kernel_directory->pages[0x300];
		*directory = ((*directory | 3) & ~0x7FFFF000) | (uint32_t)table;
	}

	pmm_update_bitmap((uint32_t)&kernel_virt_end);
	enable_paging((uint32_t)kernel_directory);

	register_interrupt_handler(14, &page_fault);
}
