

/*void main () {
    char* video_memory = ( char*) 0xb8000 ;
    *video_memory = 'A';
} */

#include "kernel.h"
#include "interrupt_service_routines.h"


/* Written for the older version of PROTOVIMA. 
    This code will be placed at the beginning of the object by the linker script   
 __asm__ (".pushsection .text.start\r\n" \
         "jmp main\r\n" \
         ".popsection\r\n"
         );  */



void main(){
    //unsigned char * video_memory = (unsigned char *) VGA_ADDRESS;
    init_vga(WHITE, BLACK);
    
    //unsigned int * memaddress = (unsigned int *)0x7000; 
    //char *address = (char *)0x7000;
    unsigned char myString[] = "Kernel running";
    printlnVGA(myString);

    //TODO: interrupt handling
    boot_memory_init();
    //TODO: Initialize paging
    isr_install();
    for(;;) {}  
}
