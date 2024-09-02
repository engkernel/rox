BITS 32

global _start
extern kernel_start

CODE_SEG equ 0x08
DATA_SEG equ 0x10

_start:
	mov ax, DATA_SEG
	mov ds, ax
	mov es, ax
	mov gs, ax
	mov fs, ax
	mov ss, ax
	mov ebp, 0x00200000
	mov esp, ebp

	call kernel_start
	jmp $

times 512-($-$$) db 0
