#include "interrupt_service_routines.h"

void isr_pushed(struct pushed_values* val){
        printlnVGA("Interrupt received!");
        printlnVGA(val->eax);
        return;
}


void isr_install(){
        initialize_idt();
        register_isr_handler(0, (unsigned int)1);
        register_isr_handler(1, (unsigned int)1);
                printlnVGA("IDT initialized!");

    }

__attribute__((noreturn))
void exception_handler(void);
void exception_handler() {
    __asm__ volatile ("cli; hlt"); // Completely hangs the computer
}
