#include "interrupt_service_routines.h"
#include "inline-assembly.h"
#include "kernel.h"
#include "timer.h"

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();

extern void irq0();
extern void irq1();
extern void irq2();
extern void _irq3();
extern void _irq4();
extern void _irq5();
extern void _irq6();
extern void _irq7();
extern void _irq8();
extern void _irq9();
extern void _irq10();
extern void _irq11();
extern void _irq12();
extern void _irq13();
extern void _irq14();
extern void _irq15();

extern void keyboard_handler_int();
extern void general_keyboard_handler(unsigned char scancode);
extern int keyboard_in_use;

void *irq_routines[16] ;

int capsLock = 0; //boolean
volatile unsigned int CountDown = 20;

const char* exception_messages[] = 
{
	"Division by zero",
	"Debug",
	"Non maskable interrupt",
	"Breakpoint",
	"Into detected overflow",
	"Out of bounds",
	"Invalid opcode",
	"No coprocessor",
	"Double fault",
	"Coprocessor segment overrun",
	"Bad TSS",
	"Segment not present",
	"Stack fault",
	"General protection fault",
	"Page fault",
	"Unknown interrupt",
	"Coprocessor fault",
	"Alignment check",
	"Machine check",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
};


// This gets called from our ASM interrupt handler stub. Jamesmolloy.co.uk.
void irq_handler(struct pushed_values* regs){
	// print_int_as_char(regs->int_no, 10);

	if(regs->int_no < 32 ){
		printlnVGA("IRQ handled");
		char *str = exception_messages[regs->int_no];
		printlnVGA(str);
		inb(0x60); //xwris to inb skalwnei xwris na dexetai deutero interrupt.
	}

	if(regs->int_no == 32){
		timer_handler();
		inb(0x60);
	}

	if(regs->int_no == 33){
		unsigned char scancode = inb(0x60); //xwris to inb skalwnei xwris na dexetai deutero interrupt.
		general_keyboard_handler((long) scancode);
	}

	if(regs->int_no >= 40){
      	// Send reset signal to slave.
     	outb(0xA0, 0x20);
  	}

    outb(0x20, 0x20);
}

void isr_pushed(struct pushed_values* val){
    printlnVGA("Interrupt received");
    char *str = exception_messages[val->int_no];
    printlnVGA(str);
    println_serious_error("Halting CPU");
    return;
}


 void irq_install(){
	register_isr_handler(32, (unsigned int)irq0);

	// Keyboard
	register_isr_handler(33, (unsigned int)irq1);

	// System call IRQ
	register_isr_handler(34, (unsigned int)irq2);

	register_isr_handler(35, (unsigned int)_irq3);
	register_isr_handler(36, (unsigned int)_irq4);
	register_isr_handler(37, (unsigned int)_irq5);
	register_isr_handler(38, (unsigned int)_irq6);
	register_isr_handler(39, (unsigned int)_irq7);
	register_isr_handler(40, (unsigned int)_irq8);
	register_isr_handler(41, (unsigned int)_irq9);
	register_isr_handler(42, (unsigned int)_irq10);
	register_isr_handler(43, (unsigned int)_irq11);
	register_isr_handler(44, (unsigned int)_irq10);
	register_isr_handler(45, (unsigned int)_irq12);
	register_isr_handler(46, (unsigned int)_irq13);


	/*irq_install_handler(32, (unsigned int)irq0);

	// Keyboard
	irq_install_handler(33, (unsigned int)irq1);

	// System call IRQ
	irq_install_handler(34, (unsigned int)irq2);

	irq_install_handler(35, (unsigned int)_irq3);
	irq_install_handler(36, (unsigned int)_irq4);
	irq_install_handler(37, (unsigned int)_irq5);
	irq_install_handler(38, (unsigned int)_irq6);
	irq_install_handler(39, (unsigned int)_irq7);
	irq_install_handler(40, (unsigned int)_irq8);
	irq_install_handler(41, (unsigned int)_irq9);
	irq_install_handler(42, (unsigned int)_irq10);
	irq_install_handler(43, (unsigned int)_irq11);
	irq_install_handler(44, (unsigned int)_irq10);
	irq_install_handler(45, (unsigned int)_irq12);
	irq_install_handler(46, (unsigned int)_irq13);

	*/
    unsigned char curmask_master = inb(0x21);
    outb(0x21, curmask_master & 0xFD);
    printlnVGA("IRQ installed");


        return;
    }


void isr_install(){

        register_isr_handler(0, (unsigned int)isr0);
        register_isr_handler(1, (unsigned int)isr1);
	    register_isr_handler(2, (unsigned int)isr2);
	    register_isr_handler(3, (unsigned int)isr3);
	    register_isr_handler(4, (unsigned int)isr4);
	    register_isr_handler(5, (unsigned int)isr5);
	    register_isr_handler(6, (unsigned int)isr6);
	    register_isr_handler(7, (unsigned int)isr7);
	    register_isr_handler(8, (unsigned int)isr8);
	    register_isr_handler(9, (unsigned int)isr9);
	    register_isr_handler(10, (unsigned int)isr10);
	    register_isr_handler(11, (unsigned int)isr11);
	    register_isr_handler(12, (unsigned int)isr12);


        printlnVGA("IDT initialized");
        return;
    }

void irq_install_handler(unsigned int irq, void (*handler)(struct pushed_values *stk))
{
	irq_routines[irq] = handler;
}


__attribute__((noreturn))
void exception_handler(void);
void exception_handler() {
    __asm__ volatile ("cli; hlt"); // Completely hangs the computer
}
