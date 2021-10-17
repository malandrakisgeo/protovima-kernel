#include "interrupt_service_routines.h"

void isr_pushed(struct pushed_values* val){
        printlnVGA("Interrupt received");
        unsigned char *str;
        str = itoa(val->eax * 4 / 1024, str, 10); 
                printlnVGA(str);

        //printlnVGA(str);
        println_serious_error("Halting CPU");
        //printlnVGA();
        //return;

}

extern void isr0();
extern void isr1();

void isr_install(){
        register_isr_handler(0, (unsigned int)isr0);
        register_isr_handler(1, (unsigned int)isr1);
        initialize_idt();
        printlnVGA("IDT initialized");

         int l = 1/0;

        return;
    }

__attribute__((noreturn))
void exception_handler(void);
void exception_handler() {
    __asm__ volatile ("cli; hlt"); // Completely hangs the computer
}
