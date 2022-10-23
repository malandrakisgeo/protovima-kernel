
#include "kernel.h"
#include "interrupt_service_routines.h"


/* Written for the older version of PROTOVIMA. 
    This code will be placed at the beginning of the object by the linker script   
 __asm__ (".pushsection .text.start\r\n" \
         "jmp main\r\n" \
         ".popsection\r\n"
         );  */

extern start_terminal();

int foreground_process = 0;
int calling_foreground_process = 0;

void main(){
    init_vga(WHITE, BLACK);
    
    //unsigned int * memaddress = (unsigned int *)0x7000; 
    //char *address = (char *)0x7000;
    unsigned char myString[] = "Kernel running";
    printlnVGA(myString);
    printlnVGA("");

    boot_memory_init();
    initialize_paging();

    printlnVGA("");

    PIC_remap();
    initialize_pic();
    initialize_idt();
    isr_install();
    irq_install();
       // int l = 1/0; //test for division-by-zero exception
       //keyboard_in_use = 1;

       start_terminal();

    while(1) __asm__("hlt\n\t");

}
