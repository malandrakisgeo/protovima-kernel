#pragma once

#include "kernel.h"


 unsigned short vga_entry(unsigned char ch, uint8 fore_color, uint8 back_color);

//clear video buffer array
 void clear_vga_buffer(uint16 **buffer, uint8 fore_color, uint8 back_color);

//initialize vga buffer
 void init_vga(uint8 fore_color, uint8 back_color);




 int println(unsigned char *text);

 void printlnVGA(unsigned char *msg);

 void remove_written_message_before_newline(unsigned char *msg);

 /*void writechar(unsigned char c, unsigned char forecolour, unsigned char backcolour, int x, int y);

 void writeline(unsigned char *msg);
*/