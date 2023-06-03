[bits 16]
[org 0x7c00]

global _start
_start:
    cli
    mov ah,0x01
    mov ch, 0x03F
    int 0x10 ;lines 7-9 disable the cursor
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, MEM_POS 
    mov ax, ERROR_MSG2
    mov [BOOT_DRIVE], dl; Boot drive passed to us by the BIOS
    mov dh, 30        ; Number of sectors (kernel.bin) to read from disk
                        ; 17*512 allows for a kernel.bin up to 8704 bytes
    mov bx, MEM_POS      ; Load Kernel to ES:BX = 0x0000:0x9000  or 0x0000:0xA000 or.... 
    call load_kernel

    call a20_gate_fast
    ;push es
    call do_e820;
    ;pop es
    cli
    xor eax,eax
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 1
    mov cr0, eax  ;enables paging. To teleutaio bit tou CR0 einai to PE (paging enabled)
    jmp dword CODE_SEG:init_pm



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
disk_error :
    mov bx , ERROR_MSG2
    ;call print_string
    hlt

; prints a null - terminated string pointed to by EDX
;print_string :
;    pusha
;    push es                   ;Save ES on stack and restore when we finish
;
;    push VIDEO_MEMORY_SEG     ;Video mem segment 0xb800
;    pop es
;    xor di, di                ;Video mem offset (start at 0)
;print_string_loop :
 ;   mov al , [ bx ] ; Store the char at BX in AL
;    mov ah , WHITE_ON_BLACK ; Store the attributes in AH
;    cmp al , 0 ; if (al == 0) , at end of string , so
 ;   je print_string_done ; jump to done
 ;   mov word [es:di], ax ; Store char and attributes at current
        ; character cell.
;    add bx , 1 ; Increment BX to the next char in string.
 ;   add di , 2 ; Move to next character cell in vid mem.
;    jmp print_string_loop ; loop around to print the next char.

;print_string_done :
  ;  pop es                    ;Restore ES that was saved on entry
  ;  popa
  ;  ret ; Return from the function


[bits 32]
init_pm:
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, MEM_POS
    mov esp, ebp

    call dword MEM_POS
    cli
loopend:           
    hlt
    jmp loopend

[bits 16]
; Variables
ERROR_MSG        db "Error!" , 0
ERROR_MSG2        db "44" , 0

BOOT_DRIVE:      db 0

VIDEO_MEMORY_SEG equ 0xb800
WHITE_ON_BLACK   equ 0x0f
MEM_POS equ 0xB000 ;;An to allakseis kai katalhsei sto gamo tou karagkiozh, 
;tha emfanizontai mono prokathorismenes times sta println. Toutestin, mono o,ti exei 
;mpei apo ton compiler sto stack, kai oxi o,ti tha evgaine sto runtime

%include "a20.asm"
%include "gdt.asm"
%include "e8202.asm"

times 510-($-$$) db 0
db 0x55
db 0xAA
