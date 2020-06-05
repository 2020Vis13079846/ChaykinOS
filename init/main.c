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
#include <chaykinos/pmm.h>
#include <chaykinos/pit.h>

extern void A20_init(void);

void start_kernel(void) {
	tty_init();
	tty_printf("TTY Initialized.\n");
	gdt_init();
	tty_printf("GDT Initialized.\n");
	idt_init();
	tty_printf("IDT Initialized.\n");
	A20_init();
	tty_printf("A20 line is on.\n");
}

void init_kernel(multiboot_info_t* mbt) {
	pmm_init(mbt);
	tty_printf("Physical Memory Manager Initialized.\n");
	asm volatile("cli");
	pit_init();
	tty_printf("PIT Initialized.\n");
	asm volatile("sti");
}

void main(uint32_t magic_number, multiboot_info_t* mbt) {
	tty_printf("ChaykinOS kernel is loaded.\n");
	if (magic_number != MULTIBOOT_BOOTLOADER_MAGIC) {
		panic("Invalid magic number: 0x%x\n", magic_number);
	}
	init_kernel(mbt);
	tty_printf("ChaykinOS is initialized.\n");
	tty_printf("Hello, world!\n");
}
