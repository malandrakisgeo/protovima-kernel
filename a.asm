[bits 16]
[org 0x7c00]
; where to load the kernel to
KERNEL_OFFSET equ 0x1000

	mov ax,0x7c00
	mov ss,ax 
	xor ax,ax
; BIOS sets boot drive in 'dl'; store for later use
mov [BOOT_DRIVE], dl

; setup stack
mov bp, 0x9000
mov sp, bp
call do_e820
call load_kernel
call a20_gate_fast

call switch_to_32bit

jmp $

%include "disk.asm"
%include "gdt.asm"
%include "switch.asm"
%include "msgutil.asm"
%include "a20.asm"
;%include "E820mem.asm"
%include "e8202.asm"
;%include "memsimplified.asm"

[bits 16]
load_kernel:
    mov bx, KERNEL_OFFSET ; bx -> destination
    mov dh, 2             ; dh -> num sectors
    mov dl, [BOOT_DRIVE]  ; dl -> disk
    call disk_load
    ret

[bits 32]
BEGIN_32BIT:
  mov eax, A20errorMsg
  mov dword [dword 0x00000dd00], eax 
    
    call KERNEL_OFFSET ; give control to the kernel
    ;jmp CODE_SEG:0x9000 ;for v2
    ;jmp $ ; loop in case kernel returns

; boot drive variable
strMuf dw "ER", 0
BOOT_DRIVE db 0
; padding
times 510 - ($-$$) db 0

; magic number
dw 0xaa55
