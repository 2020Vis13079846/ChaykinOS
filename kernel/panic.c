#include <chaykinos/panic.h>

void panic(char* error, ...) {
	va_list args;
	va_start(args, error);
	asm volatile("cli");
	uint32_t eax, ebx, ecx, edx, esi, edi, esp, ebp;
	asm ("mov %%eax, %0; mov %%ebx, %1; mov %%ecx, %2; mov %%edx, %3; mov %%esi, %4; mov %%edi, %5; mov %%esp, %6; mov %%ebp, %7": "=g"(eax), "=g"(ebx), "=g"(ecx), "=g"(edx), "=g"(esi), "=g"(edi), "=g"(esp), "=g"(ebp));
	tty_printf("kernel: panic(): ");
	tty_vprintf(error, args);
	va_end(args);
	tty_printf("Registers:\n");
	tty_printf("eax = %x ebx = %x ecx = %x edx = %x esi = %x edi = %x esp = %x ebp = %x\n", eax, ebx, ecx, edx, esi, edi, esp, ebp);
	tty_printf("Halting.\n");
	asm volatile("hlt");
}
