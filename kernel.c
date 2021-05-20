

/*void main () {
    char* video_memory = ( char*) 0xb8000 ;
    *video_memory = 'A';
} */

#include "kernel.h"
#include "vgaTextUtility.c"




//initialize vga buffer
void init_vga(uint8 fore_color, uint8 back_color)
{
  vga_buffer = (uint16*)VGA_ADDRESS;  //point vga_buffer pointer to VGA_ADDRESS 
  clear_vga_buffer(&vga_buffer, fore_color, back_color);  //clear buffer
}





void main(){
    char * video_memory = (char *) VGA_ADDRESS;
    init_vga(WHITE, BLACK);
    
    int* memaddress = (int *)0x7000; 
    //char *address = (char *)0x7000;
    char myString[] = "Hello world";
    char myStringg[] = "new line";
    printlnVGA(myString);
    printlnVGA(myStringg);

    //kernel_entry();
    for(;;) {}  
}
