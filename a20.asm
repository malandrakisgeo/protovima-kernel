    
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
    call printmsg
    call a20_gate_fast
    

;section .text
enable_A20__done: 
    xor ax,ax
    mov ah, 0x0e
    mov si, A20success
    mov al, [si]
    call printmsg
    ret
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
 
