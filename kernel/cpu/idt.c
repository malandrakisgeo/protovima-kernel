#include "idt.h"

//Twra xreiazomaste mia function pou na arxikopoiei to idt table mas. 
void load_idt(void*);

void initialize_idt(){
    idtr.limit = 256*sizeof(idt_entry_t) -1;
    idtr.base = (unsigned int)&idt;
    __asm__ volatile ("lidt %0" :: "m" (idtr)); //TODO: Kolla hur det här fungerar och varför 
    load_idt(&idtr); 
}

void register_isr_handler(int n, int handler_address){
    if(n<256){
        idt[n].kernel_cs = 0x08; 
        idt[n].isr_high = (handler_address >> 16) & 0xFFFF ; //higher 16 address
        idt[n].isr_low = handler_address & 0xFFFF; //lower 16 address
        idt[n].reserved = 0;
        idt[n].attributes = 0x8E; 
    }
}

void register_isr_handlerE(int n, int handler_address){
    if(n<256){
        idt[n].kernel_cs = 0x08; 
        idt[n].isr_high = (handler_address >> 16) & 0xFFFF ; //higher 16 address
        idt[n].isr_low = handler_address & 0xFFFF; //lower 16 address
        idt[n].reserved = 0;
        idt[n].attributes = 0xEE; //!!!! 
    }
}
