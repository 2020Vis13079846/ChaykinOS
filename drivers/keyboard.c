#include <chaykinos/interrupts.h>
#include <chaykinos/tty.h>

typedef struct {
	uint8_t scancode;
	char keycode;
} char_t;

typedef struct {
	uint8_t exists;
	uint8_t release;
	char keycode;
} kbd_char_t;

char_t keyboard_default[] = {
	{0x01, 0}, {0x3b, 0}, {0x3c, 0}, {0x3d, 0}, {0x3e, 0}, {0x3f, 0}, {0x40, 0}, {0x41, 0}, {0x42, 0}, {0x43, 0}, {0x44, 0}, {0x57, 0}, {0x58, 0}, {0x46, 0}, {0x53, '\b'},
	{0x29, '`'}, {0x2, '1'}, {0x3, '2'}, {0x4, '3'}, {0x5, '4'}, {0x6, '5'}, {0x7, '6'}, {0x8, '7'}, {0x9, '8'}, {0xa, '9'}, {0xb, '0'}, {0xc, '-'}, {0xd, '='}, {0xe, '\b'},
	{0xf, '\t'}, {0x10, 'q'}, {0x11, 'w'}, {0x12, 'e'}, {0x13, 'r'}, {0x14, 't'}, {0x15, 'y'}, {0x16, 'u'}, {0x17, 'i'}, {0x18, 'o'}, {0x19, 'p'}, {0x1a, '['}, {0x1b, ']'}, {0x2b, '\\'},
	{0x3a, 0}, {0x1e, 'a'}, {0x1f, 's'}, {0x20, 'd'}, {0x21, 'f'}, {0x22, 'g'}, {0x23, 'h'}, {0x24, 'j'}, {0x25, 'k'}, {0x26, 'l'}, {0x27, ';'}, {0x28, '\''}, {0x1c, '\n'},
	{0x2a, 0}, {0x2c, 'z'}, {0x2d, 'x'}, {0x2e, 'c'}, {0x2f, 'v'}, {0x30, 'b'}, {0x31, 'n'}, {0x32, 'm'}, {0x33, ','}, {0x34, '.'}, {0x35, '/'}, {0x36, 0},
	{0x1d, 0}, /* fn */ {0x5b, 0}, {0x38, 0}, {0x39, ' '}, {0x5d, 0}, {0x00, '\0'}
};

static uint32_t kdb_in = 0, kbd_out = 0;
static kbd_char_t keyboard_buffer[256];
volatile bool irq_fried = false;

void keyboard_wait_irq(void) {
	while (!irq_fried);
	irq_fried = false;
}

char scancode_to_keycode(kbd_char_t ch) {
	if (!ch.exists || ch.release)
		return 0;
	for (int i = 0; i < 256; i++)
		if (keyboard_default[i].scancode == ch.keycode)
			return keyboard_default[i].keycode;
	return 0;
}

void keyboard_handler(registers_t* r) {
	static kbd_char_t ch = {1, 0, 0};
	irq_fried = true;
	uint8_t scancode = inb(0x60);
	if (scancode > 0x80 && scancode < 0xe0) {
		ch.release = 1;
		scancode -= 0x80;
	}
	if (scancode != 0xe0 && scancode != 0xe1) {
		ch.keycode = scancode;
		keyboard_buffer[(kdb_in++) % 256] = ch;
		if (kdb_in > 256 && kbd_out > 256) {
			kdb_in -= 256;
			kbd_out -= 256;
		}
		ch.keycode = 0;
		ch.release = 0;
	}
}

uint8_t keyboard_getch(void) {
	uint8_t ret = 0;
	while (!ret) {
		if (kdb_in == kbd_out)
			keyboard_wait_irq();
		ret = scancode_to_keycode((kdb_in <= kbd_out) ? (kbd_char_t){0, 0, 0} : keyboard_buffer[(kbd_out++) % 256]);
	}
	tty_putchar(ret);
	return ret;
}

void keyboard_init(void) {
	register_interrupt_handler(33, &keyboard_handler);
}
