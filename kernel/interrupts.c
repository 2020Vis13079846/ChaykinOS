#include <chaykinos/interrupts.h>
#include <asm/ports.h>

static interrupt_handler_t interrupt_handlers[IDT_NUM_ENTRIES];

bool register_interrupt_handler(int index, interrupt_handler_t handler) {
	if (index >= IDT_NUM_ENTRIES)
		return false;
	if (interrupt_handlers[index] != NULL)
		return false;
	interrupt_handlers[index] = handler;
	return true;
}

bool unregister_interrupt_handler(int index) {
	if (index > IDT_NUM_ENTRIES)
		return false;
	interrupt_handlers[index] = NULL;
	return true;
}

char* exceptions[32] = {
	"Divide-by-zeror Error",
	"Debug",
	"Non-maskable Interrupt",
	"Breakpoint",
	"Overflow",
	"Bound Range Exceeded",
	"Invalid Opcode",
	"Device Not Available",
	"Double Fault",
	"Coprocessor Segment Overrun",
	"Invalid TSS",
	"Segment Not Present",
	"Stack-Segment Fault",
	"General Protection Fault",
	"Reserved",
	"x87 Floating-Point Exception",
	"Allignment Check",
	"Machine Check",
	"SIMD Floating-Point Exception",
	"Virtualization",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Security Exception",
	"Reserved"
};

void fault_handler(registers_t* r) {
	tty_printf("System Exception: %s. System Halted!\n", exceptions[r->idt_index]);
	for (;;) asm("hlt");
}

void irq_handler(registers_t* r) {
	interrupt_handler_t handler = interrupt_handlers[r->idt_index];
	if (handler)
		handler(r);
	if (r->idt_index >= 40)
		outb(0xa0, 0x20);
	outb(0x20, 0x20);
}

void run_interrupt_handler(registers_t* r) {
	size_t idt_index = r->idt_index;
	if (idt_index < 32) {
		fault_handler(r);
		return;
	}
	if (idt_index >= 32 && idt_index <= 47) {
		irq_handler(r);
		return;
	}
	if (interrupt_handlers[r->idt_index] != NULL)
		interrupt_handlers[r->idt_index](r);
}
