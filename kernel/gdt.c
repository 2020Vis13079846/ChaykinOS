#include <chaykinos/gdt.h>

gdt_ptr_t gdt_ptr;
gdt_entry_t gdt_entries[GDT_NUM_ENTRIES];

void gdt_set_gate(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
	gdt_entries[index].base_low = (base & 0xffff);
	gdt_entries[index].base_middle = (base >> 16) & 0xff;
	gdt_entries[index].base_high = (base >> 24) & 0xff;

	gdt_entries[index].limit = (limit & 0xffff);

	gdt_entries[index].granularity = ((limit >> 16) & 0xf);
	gdt_entries[index].granularity |= (granularity & 0xf0);

	gdt_entries[index].access = access;
}

void gdt_init(void) {
	gdt_ptr.limit = (sizeof(gdt_entry_t)*GDT_NUM_ENTRIES)-1;
	gdt_ptr.base = (uint32_t)&gdt_entries;

	/* NULL Descriptor */
	gdt_set_gate(0, 0, 0, 0, 0);

	/* Code Descriptor */
	gdt_set_gate(1, 0, 0xffffffff, 0x9a, 0xcf);

	/* Data Descriptor */
	gdt_set_gate(2, 0, 0xffffffff, 0x92, 0xcf);

	gdt_flush(&gdt_ptr);
}
