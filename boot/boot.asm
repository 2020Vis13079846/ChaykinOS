; ChaykinOS boot code, loaded by GRUB.
; Here're some constants needed for GRUB.
MBALIGN  equ  1 << 0            ; Align flag.
MEMINFO  equ  1 << 1            ; Memory info flag.
FLAGS    equ  MBALIGN | MEMINFO ; Flags.
MAGIC    equ  0x1BADB002        ; Multiboot magic number.
CHECKSUM equ -(MAGIC + FLAGS)   ; Multiboot checksum.

; Multiboot settings.
section .multiboot
align 4 ; 4-byte aligned code.
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

; Start of the bootstrap stack for kernel.
section .bss
stack_bottom:
resb 65536 ; 64 KiB for the stack.
stack_top: ; Start of the stack.

KERNEL_VIRTUAL_BASE equ 0xC0000000                  ; 3GB
KERNEL_PAGE_NUMBER  equ (KERNEL_VIRTUAL_BASE >> 22) ; Kernel page direcory

section .data
align 0x1000
global _boot_page_directory
_boot_page_directory:
	; This page directory entry identity-maps the first 4MB of the 32-bit physical address space.	dd 0x00000083
	times (KERNEL_PAGE_NUMBER - 1) dd 0
	; This page directory entry defines a 4MB page containing the kernel.
	dd 0x00000083
	times (1024 - KERNEL_PAGE_NUMBER - 1) dd 0

section .text
global _loader
_loader:
	mov ecx, (_boot_page_directory - KERNEL_VIRTUAL_BASE)
	mov cr3, ecx ; Load Page directory base register.

	mov ecx, cr4
	or ecx, 0x00000010 ; Set PSE bit in CR4 to enable 4MB pages.
	mov cr4, ecx

	mov ecx, cr0
	or ecx, 0x80000000 ; Set PG bit in CR0 to enable paging.
	mov cr0, ecx

	lea ecx, [_start]
	jmp ecx

global _start
_start:
	; Disable interrupts.
	cli

	; Set up the C code stack.
	mov esp, stack_top

	; Call the global constructors.
	extern _init
	call _init
	
	; GRUB puts Multiboot structure and magic number as
	; arguments to main.
	push ebx ; Multiboot structure.
	push eax ; Magic number.

	; Calling early kernel initialization.
	extern start_kernel
	call start_kernel

	; Enter protected mode.
	mov ecx, cr0
	or cl, 1
	mov cr0, ecx

	; Running kernel

	extern main
	call main

	; If the kernel is stopped, just halt.
	cli ; Disable interrupts.
	hlt ; Halt the CPU - OS kernel is stopped.
