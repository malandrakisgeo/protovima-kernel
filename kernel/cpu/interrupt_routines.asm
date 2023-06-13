;https://alex.dzyoba.com/blog/os-interrupts/
;http://www.lowlevel.eu/wiki/Teil_5_-_Interrupts
;https://wiki.osdev.org/Interrupts
;minios&tinyos
[extern isr_pushed]
[extern irq_handler]

global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12

global irq0
global irq1
global irq2
global _irq3
global _irq4
global _irq5
global _irq6
global _irq7
global _irq8
global _irq9
global _irq10
global _irq11
global _irq12
global _irq13
global _irq14
global _irq15
global read_port
global write_port


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
    
; 2:  	Non Maskable Interrupt Exception
isr2:
    cli
    push byte 0
    push byte 2
    jmp isr_common
    
;3: Breakpoint Exception
isr3:
    cli
    push byte 0
    push byte 3
    jmp isr_common
    
;4: Into Detected Overflow Exception
isr4:
    cli
    push byte 0
    push byte 4
    jmp isr_common
    
;5: Out of Bounds Exception
isr5:
    cli
    push byte 0
    push byte 5
    jmp isr_common
    
;6: Invalid Opcode Exception
isr6:
    cli
    push byte 0
    push byte 6
    jmp isr_common
    
;7: No Coprocessor Exception
isr7:
    cli
    push byte 0
    push byte 7
    jmp isr_common

;  8: Double Fault Exception
isr8:
    cli
    push byte 8        ; Note that we DON'T push byte a value on the stack in this one!
                   ; It pushes one already! Use this type of stub for exceptions that pop error codes!
    jmp isr_common
    
;9: Coprocessor Segment Overrun Exception
isr9:
    cli
    push byte 0
    push byte 9
    jmp isr_common
    
;10: Bad TSS Exception
isr10:
    cli
    push byte 10
    jmp isr_common
    
;11: Segment Not Present Exception
isr11:
    cli
    push byte 11
    jmp isr_common
    
;12: Stack Fault Exception
isr12:
    cli
    push byte 12
    jmp isr_common
    
;13: General Protection Fault Exception
isr13:
    cli
    push byte 13
    jmp isr_common
    
;14: Page Fault Exception
isr14:
    cli
    push byte 14
    jmp isr_common
    
;15: Unknown Interrupt Exception
isr15:
    cli
    push byte 0
    push byte 15
    jmp isr_common
    
;16: Coprocessor Fault Exception
isr16:
    cli
    push byte 0
    push byte 16
    jmp isr_common
    
;17: Alignment Check Exception (486+)
isr17:
    cli
    push byte 0
    push byte 17
    jmp isr_common
    
;18: Machine Check Exception (Pentium/586+)
isr18:
    cli
    push byte 0
    push byte 18
    jmp isr_common
    
;19: Reserved Exception
isr19:
    cli
    push byte 0
    push byte 19
    jmp isr_common
    
;20: Reserved Exception
isr20:
    cli
    push byte 0
    push byte 20
    jmp isr_common
    
;21: Reserved Exception
isr21:
    cli
    push byte 0
    push byte 21
    jmp isr_common
    
;22: Reserved Exception
isr22:
    cli
    push byte 0
    push byte 22
    jmp isr_common

isr_common:
    ; 1. Save CPU state
	pushad ; Pushes 32bit registers as follows: eax, ecx, edx, ebx, esp, ebi, esi, edi
	push ds
	push es
	push fs
	push gs
	;mov ax, ds ; Lower 16-bits of eax = ds.
	;push eax ; save the data segment descriptor
	mov ax, 0x10  ; kernel data segment descriptor
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov eax,esp
	push eax
	mov eax, isr_pushed
	
    ; 2. Call C handler
	call eax
	
    ; 3. Restore state
	xor eax,eax
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


irq_common_stub:
	pushad
	push ds
	push es
	push fs
	push gs
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov eax, esp
	push eax ;push pointer to stack as argument
	mov eax, irq_handler
	call eax
	pop eax
	pop gs
	pop fs
	pop es
	pop ds
	popad
	add esp, 8
	sti
	iretd


; 32: IRQ0 (Timer)
irq0: 
	cli
	push byte 0
	push byte 32
	jmp irq_common_stub
	ret;
    
 ; 33: IRQ1 (Keyboard)
irq1:
	cli
	push byte 0
	push  byte 33
	jmp irq_common_stub
	ret;

; 34: IRQ2 (PIC cascading)
irq2:
	cli
	push byte 0
	push byte 34
	jmp irq_common_stub
	ret;

	
; 35: IRQ3	
_irq3:
	cli
	push byte 0
	push byte 35
	jmp irq_common_stub
	
; 36: IRQ4	
_irq4:
	cli
	push byte 0
	push byte 36
	jmp irq_common_stub
	
; 37: IRQ5	
_irq5:
	cli
	push byte 0
	push byte 37
	jmp irq_common_stub
	
; 38: IRQ6	
_irq6:
	cli
	push byte 0
	push byte 38
	jmp irq_common_stub
	
; 39: IRQ7	
_irq7:
	cli
	push 0
	push 39
	jmp irq_common_stub
	
; 40: IRQ8	(System clock)
_irq8:
	cli
	push 0
	push 40
	jmp irq_common_stub
	
; 41: IRQ9	
_irq9:
	cli
	push 0
	push 41
	jmp irq_common_stub
	
; 42: IRQ10	(Network interface)
_irq10:
	cli
	push 0
	push 42
	jmp irq_common_stub
	
; 43: IRQ11	
_irq11:
	cli
	push 0
	push 43
	jmp irq_common_stub
	
; 44: IRQ12 (PS/2 mouse)
_irq12:
	cli
	push 0
	push 44
	jmp irq_common_stub
	
; 45: IRQ13
_irq13:
	cli
	push 0
	push 45
	jmp irq_common_stub
	
; 46: IRQ14
_irq14:
	cli
	push 0
	push 46
	jmp irq_common_stub
	
; 47: IRQ15
_irq15:
	cli
	push 0
	push 47
	jmp irq_common_stub
		

;arg: int, port number.
read_port:
    mov edx, [esp + 4]
    in al, dx
    ret

; arg: int, (dx)port number
;      int, (al)value to write
write_port:
    mov   edx, [esp + 4]
    mov   al, [esp + 4 + 4]
    out   dx, al
    ret
 

global load_idt


load_idt:
    mov edx, [esp + 4]
    lidt [edx]
    sti
    ret