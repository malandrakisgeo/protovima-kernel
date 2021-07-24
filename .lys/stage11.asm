[ org 0x7c00 ]
KERNEL_OFFSET equ 0x1000 ; This is the memory offset to which we will load our kernel
mov [ BOOT_DRIVE ], dl ; BIOS stores our boot drive in DL , so it ’s
; best to remember this for later.
mov bp , 0x9000 ; Set -up the stack.
mov sp , bp
call load_kernel ; Load our kernel
call switch_to_pm ; Switch to protected mode , from which
; we will not return
jmp $


[ bits 16]
; load_kernel
load_kernel :
mov bx , MSG_LOAD_KERNEL ; Print a message to say we are loading the kernel
call print_string
mov bx , KERNEL_OFFSET ; Set -up parameters for our disk_load routine , so
mov dh , 15 ; that we load the first 15 sectors ( excluding
mov dl , [BOOT_DRIVE] ; the boot sector ) from the boot disk ( i.e. our
call disk_load 
ret


disk_load:
  pusha           ; Be nice and push registers so we can restore thier state afterwards
  push dx         ; Store DX on stack so later we can recall
                  ; how many sectors were request to be read,
                  ; even if it is altered in the meantime
  mov ah, 0x02    ; BIOS read sector function
  mov al, dh      ; Read DH sectors
  mov ch, 0x00    ; Select cylinder 0
  mov dh, 0x00    ; Select head 0
  mov cl, 0x02    ; Start reading from second sector (ie.
                  ; after the boot sector)
  int 0x13        ; BIOS interrupt

  ;jc disk_error   ; Jump if error (ie. carry flag set)

  pop dx          ; Restore DX from the stack
  popa            ; Pop register state saved at the begining
  ret


switch_to_pm:
cli                   ; We must switch of interrupts until we have
                      ; set-up the protected mode interrupt vector
                      ; otherwise interrupts will run riot.

lgdt [gdt_descriptor] ; Load our global descriptor table, which defines
                      ; the protected mode segments (e.g. for code and data)

mov eax , cr0         ; To make the switch to protected mode, we set
or eax, 0x1           ; the first bit of CR0, a control register
mov cr0 , eax

jmp CODE_SEG:init_pm  ; Make a far jump (i.e. to a new segment) to our 32-bit
                      ; code. This also forces the CPU to flush its cache of
                      ; pre-fetched and real-mode decoded instructions, which can
                      ; cause problems.



[ bits 32]

init_pm: 
mov ax, DATA_SEG      ; Now in PM , our old segments are meaningless ,
mov ds, ax            ; so we point our segment registers to the
mov ss, ax            ; data selector we defined in our GDT
mov es, ax 
mov fs, ax 
mov gs, ax

mov ebp, 0x90000      ; Update our stack position so it is right
mov esp , ebp         ; at the top of the free space.


; This is where we arrive after switching to and initialising protected mode.
BEGIN_PM:
call KERNEL_OFFSET ; 
jmp $ ; Hang.



; Global variables
BOOT_DRIVE db 0
MSG_REAL_MODE db " Started in 16 - bit Real Mode " , 0
MSG_PROT_MODE db " Successfully landed in 32 - bit Protected Mode " , 0
MSG_LOAD_KERNEL db " Loading kernel into memory. " , 0
; Bootsector padding
times 510 -( $ - $$ ) db 0
dw 0 xaa55