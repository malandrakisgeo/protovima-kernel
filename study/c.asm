

global _start
; where to load the kernel to
KERNEL_OFFSET equ 0x1000

_start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x8000      ; Stack pointer at SS:SP = 0x0000:0x8000
    mov [BOOT_DRIVE], dl; Boot drive passed to us by the BIOS
    mov dh, 17          ; Number of sectors (kernel.bin) to read from disk
                        ; 17*512 allows for a kernel.bin up to 8704 bytes
    mov bx, 0x9000      ; Load Kernel to ES:BX = 0x0000:0x9000


; setup stack
;mov bp, 0x9000
;mov sp, bp
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
                        ; load DH sectors to ES:BX from drive DL
    push dx             ; Store DX on stack so later we can recall
                        ; how many sectors were request to be read ,
                        ; even if it is altered in the meantime
    mov ah , 0x02       ; BIOS read sector function
    mov al , dh         ; Read DH sectors
    mov ch , 0x00       ; Select cylinder 0
    mov dh , 0x00       ; Select head 0
    mov cl , 0x02       ; Start reading from second sector ( i.e.
                        ; after the boot sector )
    int 0x13            ; BIOS interrupt
    jc disk_error       ; Jump if error ( i.e. carry flag set )
    pop dx              ; Restore DX from the stack
    cmp dh , al         ; if AL ( sectors read ) != DH ( sectors expected )
    jne disk_error      ; display error message
    ret

;[bits 32]
;BEGIN_32BIT:
 ; mov eax, A20errorMsg
 ; mov dword [dword 0x00000dd00], eax 
    
    ;call main ; give control to the kernel
    ;jmp CODE_SEG:0x9000 ;for v2
    ;jmp $ ; loop in case kernel returns

; boot drive variable
strMuf dw "ER", 0
BOOT_DRIVE db 0
; padding
times 510 - ($-$$) db 0

; magic number
dw 0xaa55
