#include <asm/serial.h>
#include <asm/ports.h>

uint16_t ports[4] = { 0x3f8, 0x2f8, 0x3e8, 0x2e8 };

void serial_port_init(uint16_t port) {
	outb(port+1, 0x00);
	outb(port+3, 0x80);
	outb(port, 0x03);
	outb(port+1, 0x00);
	outb(port+3, 0x03);
	outb(port+2, 0xc7);
	outb(port+4, 0x0b);
}

int serial_recieved(uint16_t port) {
	return inb(port+5) & 1;
}

char serial_read(uint16_t port) {
	while (!serial_recieved(port));
	return inb(port);
}

int is_transmit_empty(uint16_t port) {
	return inb(port+5) & 0x20;
}

void serial_write(uint16_t port, char ch) {
	while (!is_transmit_empty(port));
	outb(port, ch);
}
