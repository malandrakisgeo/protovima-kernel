;https://alex.dzyoba.com/blog/os-interrupts/
;http://www.lowlevel.eu/wiki/Teil_5_-_Interrupts
;https://wiki.osdev.org/Interrupts
;minios&tinyos
[extern isr_pushed]

isr_common:
    ; 1. Save CPU state
	xor eax,eax
	mov eax,0xfff
	pushad ; Pushes 32bit registers as follows: eax, ecx, edx, ebx, esp, ebi, esi, edi
	push ds
	push es
	push fs
	push gs
	mov ax, ds ; Lower 16-bits of eax = ds.
	push eax ; save the data segment descriptor
	mov ax, 0x10  ; kernel data segment descriptor
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov eax, isr_pushed
	
    ; 2. Call C handler
	call eax
	
    ; 3. Restore state
	pop eax 
	pop gs 
	pop fs 
	pop es 
	pop ds
	popad
	add esp, 8 ; TODO: Des an xreiazetai h an to kanei to iret outws h allws.
	sti
	iret ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP, kai epistrefei ston diakekommeno kwdika
    
    ;H diafora metaksu iret kai ret egkeitai sto oti to iret einai prosarmosmeno gia interrupts. isws auksanei ton esp kata 8
    ;https://pdos.csail.mit.edu/6.828/2008/readings/i386/s09_06.htm

global isr0
global isr1

isr0:			;division by zero
	cli
	push byte 0 
	push byte 0
	jmp isr_common
	ret;

isr1:		;non maskable interrupt
	cli
	push byte 1
	push byte 1
	jmp isr_common
	ret;
