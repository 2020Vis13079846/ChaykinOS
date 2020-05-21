%macro error_code_handler 1
global interrupt_handler_%1
interrupt_handler_%1:
	cli
	push %1
	jmp default_interrupt_handler
%endmacro

%macro no_error_code_handler 1
global interrupt_handler_%1
interrupt_handler_%1:
	cli
	push 0
	push %1
	jmp default_interrupt_handler
%endmacro

no_error_code_handler 0
no_error_code_handler 1
no_error_code_handler 2
no_error_code_handler 3
no_error_code_handler 4
no_error_code_handler 5
no_error_code_handler 6
no_error_code_handler 7
error_code_handler 8
no_error_code_handler 9
error_code_handler 10
error_code_handler 11
error_code_handler 12
error_code_handler 13
error_code_handler 14
no_error_code_handler 15
no_error_code_handler 16
error_code_handler 17
no_error_code_handler 18
no_error_code_handler 19
no_error_code_handler 20
no_error_code_handler 21
no_error_code_handler 22
no_error_code_handler 23
no_error_code_handler 24
no_error_code_handler 25
no_error_code_handler 26
no_error_code_handler 27
no_error_code_handler 28
no_error_code_handler 29
no_error_code_handler 30
no_error_code_handler 31

no_error_code_handler 32
no_error_code_handler 33
no_error_code_handler 34
no_error_code_handler 35
no_error_code_handler 36
no_error_code_handler 37
no_error_code_handler 38
no_error_code_handler 39
no_error_code_handler 40
no_error_code_handler 41
no_error_code_handler 42
no_error_code_handler 43
no_error_code_handler 44
no_error_code_handler 45
no_error_code_handler 46
no_error_code_handler 47

extern run_interrupt_handler

default_interrupt_handler:
	pusha
	push ds
	push es
	push fs
	push gs
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov eax, esp
	push eax
	call run_interrupt_handler
	pop eax
	pop gs
	pop fs
	pop es
	pop ds
	popa
	add esp, 8
	iret
