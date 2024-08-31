BITS 16
ORG 0x7c00

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

_start:
	jmp short start ; directly jump to start
	nop		; nop is for make alignement

times 33 db 0 ; 33 byte is for BPB Bios for future use

start:
	jmp 0: stage1

stage1:
	cli
	mov ax, 0x00
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov sp, ax ; sp require direclty be set
	sti

.enable_A20:
	; Fast A20 Gate 
	in al, 0x92
	or al, 2
	out 0x92, al

.enter_protected:
	cli
	lgdt[gdt_desc]
	xor eax, eax ; empty eax register for clean setup
	mov eax, cr0
	or al, 0x1
	mov cr0, eax
		
	jmp CODE_SEG:load32

; GDT Descriptor Definition
gdt_start:

; GDT null segment 2 dword
gdt_null:
	dd 0x0
	dd 0x0

; 0x08
gdt_code: ; Kernel Code Segment
	dw 0xffff ; Segment Len(0-15)
	dw 0	  ; Segment Base(16-31)
	db 0	  ; Low Base (16-23)
	dw 0x9a   ; Access byte include type and how access(8-15)
	db 11001111b ; high and low 4 bit flag(16-23)
	db 0	  ; High Base(24-31)

; 0x10
gdt_data: ; Kernel Data Segment
	dw 0xffff ; Segment Len(0-15)
	dw 0	  ; Segment Base(16-31)
	db 0	  ; Low Base (16-23)
	dw 0x92   ; Access byte include type and how access(8-15)
	db 11001111b ; high and low 4 bit flag(16-23)
	db 0	  ; High Base(24-31)
gdt_end:
gdt_desc:
	dw gdt_end - gdt_start - 1
	dd gdt_start

[BITS 32]
load32:
	jmp $

times 510-($-$$) db 0
dw 0xAA55
