#include "idt.h"

//Twra xreiazomaste mia function pou na arxikopoiei to idt table mas. 

void initialize_idt(){
    idtr.limit = 256*sizeof(idt_entry_t);
    idtr.base = (unsigned int)&idt;
}

void register_isr_handler(int n, int handler_address){
    if(n<256){
        idt[n].kernel_cs = 123; //TODO: Kernel address
        idt[n].isr_high = (handler_address >> 16) & 0xFFFF ; //higher 16 address
        idt[n].isr_low = handler_address & 0xFFFF; //lower 16 address
    }
}