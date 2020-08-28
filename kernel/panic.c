#include <chaykinos/panic.h>
#include <chaykinos/tty.h>

void panic(char *error, ...) {
	va_list args;
	va_start(args, error);
	__asm__ volatile("cli");
	uint32_t eax, ebx, ecx, edx, esi, edi, esp, ebp;
	__asm__ ("mov %%eax, %0; mov %%ebx, %1; mov %%ecx, %2; mov %%edx, %3; mov %%esi, %4; mov %%edi, %5; mov %%esp, %6; mov %%ebp, %7": "=g"(eax), "=g"(ebx), "=g"(ecx), "=g"(edx), "=g"(esi), "=g"(edi), "=g"(esp), "=g"(ebp));
	tty_printf("kernel: panic(): ");
	tty_vprintf(error, args);
	va_end(args);
	tty_printf("Registers:\n");
	tty_printf("eax = 0x%x ebx = 0x%x ecx = 0x%x edx = 0x%x esi = 0x%x edi = 0x%x esp = 0x%x ebp = 0x%x\n", eax, ebx, ecx, edx, esi, edi, esp, ebp);
	tty_printf("Halting.\n");
	__asm__ volatile("hlt");
}
