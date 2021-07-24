[BITS 16]

KERNEL_LOAD_SEGMENT     EQU 0x100

;set data segment   ; the code segment is already set by the jmp in real mode in the bootloader.stage2
    mov ax, KERNEL_LOAD_SEGMENT
    mov ds, ax

    ;hide cursor
    mov ah, 0x01
    mov cx, 0x2607
    int 0x10

    ;move cursor at top left position
    mov ah, 0x02
    xor bx, bx
    xor dx, dx
    int 0x10

    ;clear screen
    mov ah, 0x06
    xor al, al
    xor bx, bx
    mov bh, 0x07
    xor cx, cx
    mov dh, 24
    mov dl, 79
    int 0x10

    mov si, kernel_hello_world_string
    call BIOS_print_string

    ;enable A20 gate
    in al, 0x92
    or al, 2
    out 0x92, al

    ;disable interrupts
    cli

    ;load gdt
    lgdt [gdt_desc]

    ;set bit 1(32bit protected mode) in cr0
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp CODE_SEGMENT:main32

BIOS_print_newline:
    ; removed for clarity
    ret
BIOS_print_string:
    ; removed for clarity
    ret

kernel_hello_world_string:
    db "[KERNEL] Hello world! Entering 32-bit protected mode", 0x0


[BITS 32]

gdt_start:
gdt_null_segment:
    dq 0
gdt_code_segment:
    dw 0xFFFF
    dw 0x1000
    db 0
    db 10011010b
    db 11001111b
    db 0
gdt_data_segment:
    dw 0xFFFF
    dw 0x1000
    db 0
    db 10010010b
    db 11001111b
    db 0
gdt_end:

gdt_desc:
    db gdt_end - gdt_start
    dw gdt_start

CODE_SEGMENT equ gdt_code_segment - gdt_start
DATA_SEGMENT equ gdt_data_segment - gdt_start

print32:
    ; removed for clarity
    ret

main32:
    jmp $ ;;; WITH THIS JUMP THE REBOOTING IS GONE
    mov ax, DATA_SEGMENT
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov ebp, 0x2000
    mov esp, ebp

    mov ebx, message
    ;call print32

    jmp $

message:
    db "32 bit protected mode entry point", 0

;pad
times 512 - ($ - $$) db 0 
