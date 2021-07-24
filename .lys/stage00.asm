[BITS 16] ;endast för -f bin
[ORG 0x7C00] ;endast för -f bin


;section .text
_start: 
    xor ax,ax      ; We want a segment of 0 for DS for this question
    mov ds,ax      ;     Set AX to appropriate segment value for your situation
    mov es,ax      ; In this case we'll default to ES=DS
    mov bx,0x8000  ; Stack segment can be any usable memory

    cli            ; Disable interrupts to circumvent bug on early 8088 CPUs
    mov ss,bx      ; This places it with the top of the stack @ 0x80000.
    mov sp,ax      ; Set SP=0 so the bottom of stack will be @ 0x8FFFF
    sti            ; Re-enable interrupts


    cld ; clear direction flags
    mov ax, 03h  ;Clear the screen
    int 10h ; BIOS call for clearing 
    
    ;mov cs, ax ;it fucks the printing up!!!
   ; mov sp, 0x8000
    mov ah, 0x0e
    mov si, realModeMsg 
    
    call printmsg  
    
    hlt
    
    
    
;section .text    
a20_gate_fast: 
    xor ax,ax
    ;There is no guarantee that this works on a system! It works in most cases,
    in al, 0x92 ;but not always. TODO: Implement at least one more algorithm enabling A20
    or al, 2
    out 0x92, al
    call check_a20
    xchg bx, bx
    cmp  ax, 0
    jne  enable_A20__done ;jump if A20 enabled
    mov si,A20errorMsg
    jmp printmsg
    jmp a20_gate_fast
    

;section .text
enable_A20__done: 
    xor ax,ax
    mov ah, 0x0e
    mov si, A20sucess
    mov al, [si]
    jmp protected_mode
    ;jmp printmsg ;HERE
    ;jmp load_gdt
    ;call kernel_entry
    
    
    ;http://www.independent-software.com/operating-system-development-enabling-a20-line.html
    ; https://stackoverflow.com/questions/52668637/is-my-understanding-of-the-a20-line-check-code-correct
check_a20:
    pushf                                  ;Backup the current flags onto the stack
                                           ;Backup the below registers onto the stack
    push ds                                ;|
    push es                                ;|
    push di                                ;|
    push si                                ;-----

    cli                                    ;Disable interupts

    xor ax, ax                             ; ax = 0
    mov es, ax                             ;es = ax

    not ax                                 ; ax = 0xFFFF
    mov ds, ax                             ; ds = ax

    mov di, 0x0500                         ;Boot signature part one (0x55)
    mov si, 0x0510                         ;Boot signature part two (0xAA)

    mov al, byte [es:di]                   ;al = value at AA:55
    push ax                                ;Backup ax register onto the stack

    mov al, byte [ds:si]                   ;al = value at 55:AA
    push ax                                ;Backup al onto the stack

    mov byte [es:di], 0x00                 ;Memory location AA:55 = 0
    mov byte [ds:si], 0xFF                 ;Memory location at 55:AA = 0xFF

    cmp byte [es:di], 0xFF                 ;Does value at AA:55 = 0xFF? If so, this means A20 is disabled

    pop ax                                 ;Restore saved ax register
    mov byte [ds:si], al                   ;Set 55:AA to al

    pop ax                                 ;Restore ax register
    mov byte [es:di], al                   ;set AA:55 to al

    mov ax, 0                              ;Return status of this function = 0 (Disabled)
    je check_a20__exit                     ;A20 is disabled. Go to check_a20__exit

    mov ax, 1                               ;Return status of this function = 1 (Enabled)
    jmp check_a20__exit
    ret 
    
check_a20__exit:
                                           ;Backup registers
    pop si
    pop di
    pop es
    pop ds
    popf                                   ;Backup flags

    ret                                    ;Return

    
  ;FOUND ON OSDev.org. Similarly implemented on simple-x86-bootloader 
load_gdt:
    mov ax, gdtend
    mov bx, gdt
    sub ax, bx  ;compute GDT's limit
    mov WORD [gdtptr], ax

    xor eax, eax  ;compute linear of GDT
    mov ax, ds
    shl eax, 4
    xor ebx, ebx
    mov ebx, gdt
    add eax, ebx
    mov DWORD [gdtptr + 2], eax

  
  
protected_mode: 
    cli
        ;loader

    ;loaderend
    lgdt [GDT_DESCRIPTOR]     ;load  gdt
    mov eax, cr0
    or  ax, 1
    mov cr0, eax 

    xor ax,ax
    mov ax,DATA_SEG
    mov ds,ax
    mov es,ax
    mov fs,ax
    mov gs,ax
    mov ss,ax



    jmp 0x01000

    ;call kernel_entry
    ;jmp 0x08:init_pm

    ;jmp init_pm
        
        
    

    ;;functions for general use
  printmsg:
    mov al, [si]
    add si, 1
    cmp al, 0xa
    jne print_interrupt
    ret
    ; alternative way: 
    ;lodsb 
    ;or al,al
    ;jz print_interrupt
    
print_interrupt: 
    int 0x10
    jmp printmsg


    
[bits 32]
init_pm:
    mov ax, 0x10
    mov ds, ax
    mov ss, ax

    mov esp, 0x090000  ;set up stack pointer

    ;mov byte [0xB8000], 88
   ; mov byte [0xB8000+1], 0x1B

    ;call dword 0x08:0x01000  ;go to C code
    jmp 0x01000

    mov byte [0xB8000+4], 89
    mov byte [0xB8000+5], 0x1B
    jmp $
    
    
    
    
    
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

realModeMsg db "Real mode.", 0xa
A20errorMsg db "Error A20", 0xa
A20sucess db "A20 success!", 0xa

gdt: ;gdt based on simple-x86-bootloader
     db 0, 0, 0, 0, 0, 0, 0, 0
gdt_cs: ; flat model
     db 0xFF, 0xFF, 0x0, 0x0, 0x0, 10011011b, 11011111b, 0x0
gdt_ds: ; flat model
     db 0xFF, 0xFF, 0x0, 0x0, 0x0, 10010011b, 11011111b, 0x0
gdtend:
     ;---------------------------------------------------------
gdtptr:
    dw 0  ; limite
    dd 0  ; base

DATA_SEG equ gdt_ds - gdt

GDT_DESCRIPTOR: ; to be loaded by instruction lgdt
    ; The size is the size of the table subtracted by 1. This is because the
    ; maximum value of size is 65535, while the GDT can be up to 65536 bytes (a
    ; maximum of 8192 entries). Further no GDT can have a size of 0. 
    dw gdtptr - gdt - 1    
    ; The offset is the linear address of the table itself
    ;dd gdt
;--------------------------------------------------------------------

;clc 
; Switch to the BIOS (= request low memory size)
;int 0x12
;jmp 0x9d0000
 
;----------------------------------------------;
; Bootloader signature must be located
; at bytes #511 and #512.
; Fill with 0 in between.
; $  = address of the current line
; $$ = address of the 1st instruction
;----------------------------------------------;
times 510 - ($-$$) db 0
dw        0xaa55
