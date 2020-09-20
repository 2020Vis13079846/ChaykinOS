#include <chaykinos/idt.h>
#include <asm/ports.h>
#include <string.h>

#define DECLARE_HANDLER(index) extern void interrupt_handler_##index(void)
#define SET_IDT_ENTRY(index) idt_set_gate(index, (uint32_t)&interrupt_handler_##index, 0x08, 0x8e)

idt_entry_t idt_entries[IDT_NUM_ENTRIES];
idt_ptr_t idt_ptr;

/* Interrupt Service Routines */

DECLARE_HANDLER(0);
DECLARE_HANDLER(1);
DECLARE_HANDLER(2);
DECLARE_HANDLER(3);
DECLARE_HANDLER(4);
DECLARE_HANDLER(5);
DECLARE_HANDLER(6);
DECLARE_HANDLER(7);
DECLARE_HANDLER(8);
DECLARE_HANDLER(9);
DECLARE_HANDLER(10);
DECLARE_HANDLER(11);
DECLARE_HANDLER(12);
DECLARE_HANDLER(13);
DECLARE_HANDLER(14);
DECLARE_HANDLER(15);
DECLARE_HANDLER(16);
DECLARE_HANDLER(17);
DECLARE_HANDLER(18);
DECLARE_HANDLER(19);
DECLARE_HANDLER(20);
DECLARE_HANDLER(21);
DECLARE_HANDLER(22);
DECLARE_HANDLER(23);
DECLARE_HANDLER(24);
DECLARE_HANDLER(25);
DECLARE_HANDLER(26);
DECLARE_HANDLER(27);
DECLARE_HANDLER(28);
DECLARE_HANDLER(29);
DECLARE_HANDLER(30);
DECLARE_HANDLER(31);

/* Interrupt Requests */

DECLARE_HANDLER(32);
DECLARE_HANDLER(33);
DECLARE_HANDLER(34);
DECLARE_HANDLER(35);
DECLARE_HANDLER(36);
DECLARE_HANDLER(37);
DECLARE_HANDLER(38);
DECLARE_HANDLER(39);
DECLARE_HANDLER(40);
DECLARE_HANDLER(41);
DECLARE_HANDLER(42);
DECLARE_HANDLER(43);
DECLARE_HANDLER(44);
DECLARE_HANDLER(45);
DECLARE_HANDLER(46);
DECLARE_HANDLER(47);

void idt_set_gate(int index, uint64_t offset, uint16_t selector, uint8_t type) {
	idt_entries[index].offset_low = (offset & 0xffff);
	idt_entries[index].selector = selector;
	idt_entries[index].zero = 0;
	idt_entries[index].type = type;
	idt_entries[index].offset_high = (offset >> 16) & 0xffff;
}

void idt_init(void) {
	int i;
	idt_ptr.limit = (sizeof(idt_entry_t)*IDT_NUM_ENTRIES)-1;
	idt_ptr.base = (uint32_t)&idt_entries;

	idt_flush(&idt_ptr);

	for (i = 0; i < IDT_NUM_ENTRIES; i++) {
		idt_entries[i].offset_high = 0;
		idt_entries[i].offset_low = 0;
		idt_entries[i].zero = 0;
		idt_entries[i].type = 0;
		idt_entries[i].selector = 0;
	}

	SET_IDT_ENTRY(0);
	SET_IDT_ENTRY(1);
	SET_IDT_ENTRY(2);
	SET_IDT_ENTRY(3);
	SET_IDT_ENTRY(4);
	SET_IDT_ENTRY(5);
	SET_IDT_ENTRY(6);
	SET_IDT_ENTRY(7);
	SET_IDT_ENTRY(8);
	SET_IDT_ENTRY(9);
	SET_IDT_ENTRY(10);
	SET_IDT_ENTRY(11);
	SET_IDT_ENTRY(12);
	SET_IDT_ENTRY(13);
	SET_IDT_ENTRY(14);
	SET_IDT_ENTRY(15);
	SET_IDT_ENTRY(16);
	SET_IDT_ENTRY(17);
	SET_IDT_ENTRY(18);
	SET_IDT_ENTRY(19);
	SET_IDT_ENTRY(20);
	SET_IDT_ENTRY(21);
	SET_IDT_ENTRY(22);
	SET_IDT_ENTRY(23);
	SET_IDT_ENTRY(24);
	SET_IDT_ENTRY(25);
	SET_IDT_ENTRY(26);
	SET_IDT_ENTRY(27);
	SET_IDT_ENTRY(28);
	SET_IDT_ENTRY(29);
	SET_IDT_ENTRY(30);
	SET_IDT_ENTRY(31);

	SET_IDT_ENTRY(32);
	SET_IDT_ENTRY(33);
	SET_IDT_ENTRY(34);
	SET_IDT_ENTRY(35);
	SET_IDT_ENTRY(36);
	SET_IDT_ENTRY(37);
	SET_IDT_ENTRY(38);
	SET_IDT_ENTRY(39);
	SET_IDT_ENTRY(40);
	SET_IDT_ENTRY(41);
	SET_IDT_ENTRY(42);
	SET_IDT_ENTRY(43);
	SET_IDT_ENTRY(44);
	SET_IDT_ENTRY(45);
	SET_IDT_ENTRY(46);
	SET_IDT_ENTRY(47);

	outb(0x20, 0x10);
	outb(0xA0, 0x10);
	outb(0x21, 0x20);
	outb(0xA1, 0x28);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0x0);
	outb(0xA1, 0x0);
}
