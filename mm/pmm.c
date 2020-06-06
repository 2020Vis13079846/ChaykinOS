#include <chaykinos/pmm.h>
#include <chaykinos/tty.h>
#include <chaykinos/multiboot.h>
#include <string.h>

uint32_t kernel_phys_map_start, kernel_phys_map_end;

uint32_t* phys_memory_bitmap = (uint32_t*)&kernel_phys_end;
uint32_t phys_block_count = 0;
uint32_t phys_used_block_count = 0;

uint32_t phys_installed_memory_size = 0, phys_available_memory_size = 0;

static void bitmap_set(int bit) {
	phys_memory_bitmap[bit / 32] |= (1 << (bit % 32));
}

static void bitmap_unset(int bit) {
	phys_memory_bitmap[bit / 32] &= ~(1 << (bit % 32));
}

static bool bitmap_test(int bit) {
	return phys_memory_bitmap[bit / 32] & (1 << (bit & 32));
}

int pmm_find_free_block(void) {
	for (uint32_t i = 0; i < phys_block_count / 32; i++) {
		uint32_t block = phys_memory_bitmap[i];
		if (block != 0xffffffff) {
			for (uint8_t j = 0; j < 32; j++) {
				int bit = 1 << j;
				if (!(bit & block)) {
					return 32 * i + j;
				}
			}
		}
	}
	return -1;
}

uint32_t pmm_block_alloc(void) {
	if (phys_block_count - phys_used_block_count <= 0)
		return 0;
	int block = pmm_find_free_block();
	if (block == -1)
		return 0;
	bitmap_set(block);
	uint32_t addr = block * 4096;
	phys_used_block_count++;
	return addr;
}

void pmm_block_free(uint32_t addr) {
	int block = addr / 4096;
	bitmap_unset(block);
	phys_used_block_count--;
}

bool pmm_is_block_alloced(uint32_t addr) {
	int block = addr / 4096;
	return bitmap_test(block);
}

void pmm_chunk_alloc(uint64_t addr, uint64_t length) {
	int current_block = addr / 4096;
	int blocks = length / 4096;
	while (blocks-- >= 0) {
		bitmap_set(current_block++);
		phys_used_block_count--;
	}
}

void pmm_chunk_free(uint64_t addr, uint64_t length) {
	int current_block = addr / 4096;
	int blocks = length / 4096;
	while (blocks--) {
		bitmap_unset(current_block++);
		phys_used_block_count--;
	}
}

void pmm_free_memory(multiboot_info_t* mbt) {
	for (multiboot_memory_map_t* mm = (multiboot_memory_map_t*)mbt->mmap_addr; (uint32_t)mm < mbt->mmap_addr + mbt->mmap_length; mm = (multiboot_memory_map_t*)((uint32_t)mm + mm->size + sizeof(mm->size))) {
		if (mm->type == 1)
			pmm_chunk_free(mm->addr, mm->len);
	}
	bitmap_set(0);
}

void pmm_memory_map(multiboot_info_t* mbt) {
	for (multiboot_memory_map_t* mm = (multiboot_memory_map_t*)mbt->mmap_addr; (uint32_t)mm < mbt->mmap_addr + mbt->mmap_length; mm = (multiboot_memory_map_t*)((uint32_t)mm + mm->size + sizeof(mm->size))) {
		if (mm->type == 1)
			phys_available_memory_size += mm->len;
		phys_installed_memory_size += mm->len;
	}
}

void pmm_update_bitmap(uint32_t addr) {
	phys_memory_bitmap = (uint32_t*) addr;
}

void pmm_init(multiboot_info_t* mbt) {
	pmm_memory_map(mbt);
	phys_block_count = phys_installed_memory_size / 4096;
	phys_used_block_count = phys_block_count;
	memset(phys_memory_bitmap, 0xff, phys_block_count / 8);

	pmm_free_memory(mbt);
	pmm_chunk_alloc((uint32_t)&kernel_phys_start, (uint32_t)&kernel_phys_end-(uint32_t)&kernel_phys_start);
	pmm_chunk_alloc(*phys_memory_bitmap, phys_block_count);
	kernel_phys_map_start = (uint32_t)phys_memory_bitmap;
	kernel_phys_map_end = kernel_phys_map_start + (phys_block_count / 8);
}
