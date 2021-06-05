[bits 16]
switch_to_32bit:
    cli                     ; 1. disable interrupts
    lgdt [gdt_descriptor]   ; 2. load GDT descriptor
    mov eax, cr0
    or eax, 0x1             ; 3. enable protected mode
    mov cr0, eax
    jmp CODE_SEG:init_32bit ; 4. far jump.  GIA V1
    ;jmp init_32bit ;GIA V2&v3
    ;call 0x1000

[bits 32]
init_32bit:
    mov ax, DATA_SEG        ; 5. update segment registers
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000        ; 6. setup stack
    mov esp, ebp

    ;call main        ; 7. move back to a.asm 
    call 0x9000
    cli
    loopend:                ;Infinite loop when finished
        hlt
        jmp loopend ;v3
