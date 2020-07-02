#include <chaykinos/nmi.h>
#include <asm/ports.h>

void nmi_enable(void) {
	outb(0x70, inb(0x70) & 0x7f);
}

void nmi_disable(void) {
	outb(0x70, inb(0x70) | 0x80);
}
