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
resb 16384 ; 16 KiB for the stack.
stack_top: ; Start of the stack.

section .text
global _start
_start:
	; Disable interrupts.
	cli

	; Set up the C code stack.
	mov esp, stack_top

	; Running kernel.
	extern start_kernel
	call start_kernel

	extern main
	call main

	; If the kernel is stopped, just halt.
	cli ; Disable interrupts.
	hlt ; Halt the CPU - OS kernel is stopped.
