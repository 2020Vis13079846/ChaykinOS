#include <chaykinos/ssp.h>
#include <chaykinos/panic.h>

uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void) {
#if __is_kernel
	panic("Stack smashing detected.\n");
#endif
}
