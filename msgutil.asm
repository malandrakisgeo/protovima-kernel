 [bits 16]

;; printing functions for general use
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

    ;;;;;;;;;;;;;;;;;;;;;
    
realModeMsg dd "Real mode.", 0xa
A20errorMsg dd "Error A20", 0xa
A20success dd "A20 success!", 0xa
E820msg dd "E820 !", 0xa
