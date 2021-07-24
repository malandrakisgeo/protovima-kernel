section .entry

extern main
;global _start

_start:
    cld
    mov ah, 0
    int 0x13
    or ah, ah
    jnz _start

    ; load from disk
    xor ax, ax
    mov es, ax
    mov ch, ah
    mov dh, ah
    mov bx, 0x1000
    mov cl, 0x02
    mov ah, 0x02
    mov al, 0x02
    int 0x13
    or ah, ah
    jnz _start

    cli ; disable iterrupts

    ; set the ds register
    xor ax, ax
    mov ds, ax

    lgdt [gtd_desc] ; load segments table

    ; go to protected mode
    mov eax, cr0
    or eax, 0x01
    mov cr0, eax

    jmp 0x08:clear_pipe32 ; go to 32-bit code


[BITS 32]
extern main

clear_pipe32:
  ; set segment registers
  mov ax, 0x10
  mov ds, ax
  mov ss, ax

  mov esp, 0x090000 ; set up stack pointer

  mov byte [0xB8000], 88
  mov byte [0xB8000+1], 0x1B

  call main ; go to C code

  ;mov byte [0xB8000+4], 89
  ;mov byte [0xB8000+5], 0x1B
  jmp $




gdt:

gdt_null:
  dd 0
  dd 0

gdt_code:
  dw 0xFFFF
  dw 0
  db 0
  db 10011010b
  db 11001111b
  db 0

gdt_data:
  dw 0xFFFF
  dw 0
  db 0
  db 10010010b
  db 11001111b
  db 0

gdt_end:

gtd_desc:
   dw gdt_end - gdt - 1
   dd gdt

TIMES 510 - ($ - $$) db 0	; Fill the rest of sector with 0
DW 0xAA55 ; Add boot signature at the end of bootloader
