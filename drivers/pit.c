#include <chaykinos/pit.h>
#include <chaykinos/interrupts.h>
#include <asm/ports.h>

static uint64_t ticks = 0;

static void pit_handler(__attribute__((unused)) registers_t *r) {
	ticks++;
}

uint32_t pit_get_ticks(void) {
	return ticks;
}

void pit_sleep(uint32_t time) {
	uint64_t eticks = ticks + time;
	while (pit_get_ticks() < eticks);
}

void pit_init(void) {
	register_interrupt_handler(32, &pit_handler);
	int divisor = 1193180 / PIT_FREQUENCY;
	outb(0x43, 0x36);
	outb(0x40, divisor & 0xff);
	outb(0x40, divisor >> 8);
}
