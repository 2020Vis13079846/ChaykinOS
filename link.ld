ENTRY(_loader)

SECTIONS {
	. = 0xC0100000;
	kernel_phys_start = . - 0xC0000000;
	kernel_virt_start = .;
	.text : AT(ADDR(.text) - 0xC0000000) {
		*(.multiboot)
		*(.text)
		. = ALIGN(0x1000);
	}
	.rodata : AT(ADDR(.rodata) - 0xC0000000) {
		*(.rodata*)
		. = ALIGN(0x1000);
	}
	.data : AT(ADDR(.data) - 0xC0000000) {
		*(.data)
		. = ALIGN(0x1000);
	}
	.bss : AT(ADDR(.bss) - 0xC0000000) {
		*(COMMON)
		*(.bss)
		. = ALIGN(0x1000);
	}
	kernel_phys_end = . - 0xC0000000;
	kernel_virt_end = .;
}
