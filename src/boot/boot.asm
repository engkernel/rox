ORG 0x7c00
BITS 16

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

_start:
	jmp short start ; directly jump to start
	nop		; nop is for make alignement

times 33 db 0 ; 33 byte is for BPB Bios for future use

start:
	jmp 0:stage1

stage1:
	cli
	mov ax, 0x00
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov sp, 0x7c00 ; sp require direclty be set
	sti

	; load GDT to jump to protected mode
	cli
	lgdt [gdt_desc]

	mov eax, cr0
	or eax, 0x1
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
	db 0x9a   ; Access byte include type and how access(8-15)
	db 11001111b ; high and low 4 bit flag(16-23)
	db 0	  ; High Base(24-31)

; 0x10
gdt_data: ; Kernel Data Segment
	dw 0xffff ; Segment Len(0-15)
	dw 0	  ; Segment Base(16-31)
	db 0	  ; Low Base (16-23)
	db 0x92   ; Access byte include type and how access(8-15)
	db 11001111b ; high and low 4 bit flag(16-23)
	db 0	  ; High Base(24-31)
gdt_end:
gdt_desc:
	dw gdt_end - gdt_start - 1
	dd gdt_start

[BITS 32]
load32:
	; Fast A20 Gate 
	in al, 0x92
	or al, 2
	out 0x92, al

	mov eax, 1
	mov ecx, 100
	mov edi, 0x0100000
	call read_disk
	jmp CODE_SEG:0x0100000

; Read disk in LBA mode
read_disk:
	mov ebx, eax 		; Save LBA in EBX
	
	mov dx, 0x1F6		; Port to send drive and bit 24-27 of LBA
	shr eax, 24		; Get bit 24-27 in al
	or eax, 11100000b	; Set bit 6 in al for LBA mode (0xE0)
	out dx, al

	mov dx, 0x01F2		; Port to send number of sectors
	mov eax, ecx		; Get number of sectors from CL
	out dx, al

	mov dx, 0x1F3		; Port to send bit 0-7 of LBA
	mov eax, ebx		; Get LBA from EBX
	out dx, al

	mov dx, 0x1F4		; Port to send bit 8-15 of LBA
	mov eax, ebx		; Get LBA from EBX
	shr eax, 8		; Get bit 8-15 in AL
	out dx, al

	mov dx, 0x1F5		; Port to send bit 16-23 of LBA
	mov eax, ebx		; Get LBA from EBX
	shr eax, 16		; Get bit 16-23 in AL
	out dx, al

	mov dx, 0x1F7		; Command port
	mov al, 0x20		; Read with retry
	out dx, al

.read_next_sector:
	push ecx 		; save ecx value for next iteration

.retry_read:
	mov dx, 0x1F7		; we are looping so every loop set dx to 0x1F7
	in al, dx
	test al, 8		; the sector buffer requires servicing
	jz .retry_read		; until the sector buffer is ready
	
	; buffer is ready so we read 512 byte
	mov ecx, 256		; ECX is counter for INSW
	mov dx, 0x1F0		; Data port, in and out
	rep insw		; in to [RDI] 
	pop ecx			; restore ecx value for decreasing in end of iteration (100..1)
	loop .read_next_sector
	; when we read all sectors reach here
	ret

times 510-($-$$) db 0
dw 0xAA55
