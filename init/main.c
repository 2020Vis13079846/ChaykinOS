/*

	Welcome to ChaykinOS source code!
	ChaykinOS is an OS from scratch.
	It's developed by Simon Chaykin
	(game-lover).

*/

#include <chaykinos/tty.h>
#include <chaykinos/panic.h>
#include <chaykinos/multiboot.h>
#include <chaykinos/gdt.h>
#include <chaykinos/idt.h>

extern void pit_init(void);

void start_kernel(void) {
	tty_init();
	tty_printf("TTY Initialized.\n");
	gdt_init();
	tty_printf("GDT Initialized.\n");
	idt_init();
	tty_printf("IDT Initialized.\n");
}

void init_kernel(void) {
	asm volatile("cli");
	pit_init();
	tty_printf("PIT Initialized.\n");
	asm volatile("sti");
}

void main(uint32_t magic_number, multiboot_info_t* mbt) {
	if (magic_number != MULTIBOOT_BOOTLOADER_MAGIC) {
		panic("Invalid magic number: 0x%x\n", magic_number);
	}
	init_kernel();
	tty_printf("flags = 0x%x\n", mbt->flags);
	if (CHECK_FLAG(mbt->flags, 0))
		tty_printf("mem_lower = %u KB, mem_upper = %u KB\n", mbt->mem_lower, mbt->mem_upper);
	if (CHECK_FLAG(mbt->flags, 1))
		tty_printf("boot_device = 0x%x\n", mbt->boot_device);
	if (CHECK_FLAG(mbt->flags, 2))
		tty_printf("cmdline = %s\n", (char*)mbt->cmdline);
	if (CHECK_FLAG(mbt->flags, 3)) {
		multiboot_module_t* mod;
		uint32_t i;
		tty_printf("mods_count = %d, mods_addr = 0x%x\n", mbt->mods_count, mbt->mods_addr);
		for (i = 0, mod = (multiboot_module_t*)mbt->mods_addr; i < mbt->mods_count; i++, mod++)
			tty_printf(" mod_start = 0x%x, mod_end = 0x%x, cmdline = %s\n", mod->mod_start, mod->mod_end, mod->cmdline);
	}
	if (CHECK_FLAG(mbt->flags, 4) && CHECK_FLAG(mbt->flags, 5)) {
		panic("Both bits 4 and 5 are set.\n");
	}
	if (CHECK_FLAG(mbt->flags, 4)) {
		multiboot_aout_symbol_table_t* multiboot_aout_sym = &(mbt->u.aout_sym);
		tty_printf("multiboot_aout_table: tabsize = 0x%x, strsize = 0x%x, addr = 0x%x\n", multiboot_aout_sym->tabsize, multiboot_aout_sym->strsize, multiboot_aout_sym->addr);
	}
	if (CHECK_FLAG(mbt->flags, 5)) {
		multiboot_elf_section_header_table_t* multiboot_elf_sec = &(mbt->u.elf_sec);
		tty_printf("multiboot_elf_sec: num = %u, size = 0x%x, addr = 0x%x, shndx = 0x%x\n", multiboot_elf_sec->num, multiboot_elf_sec->size, multiboot_elf_sec->addr, multiboot_elf_sec->shndx);
	}
	if (CHECK_FLAG(mbt->flags, 6)) {
		uint32_t memory_installed;
		multiboot_memory_map_t* mmap;
		tty_printf("mmap_addr = 0x%x, mmap_length = 0x%x\n", mbt->mmap_addr, mbt->mmap_length);
		for (mmap = (multiboot_memory_map_t *) mbt->mmap_addr; (unsigned long) mmap < mbt->mmap_addr + mbt->mmap_length; mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + mmap->size + sizeof (mmap->size))) {
			tty_printf (" size = 0x%x, base_addr = 0x%x%x, length = 0x%x%x, type = 0x%x\n", (uint32_t) mmap->size, (uint32_t) (mmap->addr >> 32), (uint32_t) (mmap->addr & 0xffffffff), (uint32_t) (mmap->len >> 32), (uint32_t) (mmap->len & 0xffffffff), (uint32_t) mmap->type);
			memory_installed += mmap->len;
		}
		tty_printf("Install Memory: %u KB\n", memory_installed / 1024);
	}
	tty_printf("Hello, world!\n");
}
