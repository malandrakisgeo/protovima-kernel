
#include "kernel.h"
#include "interrupt_service_routines.h"
#include "process.h";
#include "memoryManager.h"
#include "vgaTextUtility.h"
#include "commands.h"


/* Written for the older version of PROTOVIMA. 
    This code will be placed at the beginning of the object by the linker script   
 __asm__ (".pushsection .text.start\r\n" \
         "jmp main\r\n" \
         ".popsection\r\n"
         );  */

extern void start_terminal();

int foreground_process = 0; //the address of the process currently appearing -either for receiving input, or for printing it.
int calling_foreground_process = 0;

void main()
{
    init_vga(WHITE, BLACK);

    //unsigned int * memaddress = (unsigned int *)0x7000;
    //char *address = (char *)0x7000;
    unsigned char myString[] = "Kernel running";
    printlnVGA(myString);
    printlnVGA("");

    boot_memory_init(); 

    printlnVGA("");

    PIC_remap();
    initialize_pic();
    initialize_idt();
    isr_install();
    irq_install();

    // int l = 1/0; //test for division-by-zero exception
    
    initialize_paging();

    start_terminal();

    while (1)
        __asm__("hlt\n\t");
}
