#include <chaykinos/panic.h>
#include <chaykinos/interrupts.h>

void page_fault(__attribute__((unused)) registers_t *r) {
	uint32_t faulting_address;
	asm volatile("mov %%cr2, %0" : "=r" (faulting_address));
	panic("Page fault at addr 0x%x\n", faulting_address);
}

void vmm_init(void) {
	register_interrupt_handler(14, &page_fault);
}
