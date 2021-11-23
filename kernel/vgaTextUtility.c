#include "kernel.h"
#include "vgaTextUtility.h" 

unsigned int xPos=0;
unsigned int yPos=0;
uint16* vga_buffer;
/*
16 bit video buffer elements(register ax)
8 bits(ah) higher : 
  lower 4 bits - forec olor
  higher 4 bits - back color

8 bits(al) lower :
  8 bits : ASCII character to print
*/




uint16 vga_entry(unsigned char ch, uint8 fore_color, uint8 back_color) 
{
  uint32 eax = 0;
  uint16 ax = 0;
  uint8 ah = 0, al = 0;

  ah = back_color;
  ah <<= 4;
  ah |= fore_color;
  ax = ah;
  ax <<= 8;
  al = ch;
  ax |= al;

  eax |= ax;
  eax <<= 16;
  return ax;
}

//clear video buffer array
void clear_vga_buffer(uint16 **buffer, uint8 fore_color, uint8 back_color)
{
  uint16 i;
  for(i = 0; i < BUFSIZE; i++){
    (*buffer)[i] = vga_entry(NULL, fore_color, back_color);
  }
}

//initialize vga buffer
void init_vga(uint8 fore_color, uint8 back_color)
{
   xPos = 0;
   yPos = 0;
  vga_buffer = (uint16*)(VGA_ADDRESS);  //point vga_buffer pointer to VGA_ADDRESS 
  clear_vga_buffer(&vga_buffer, fore_color, back_color);  //clear buffer
}



unsigned char *itoa( unsigned long value, unsigned char * str, unsigned int base )
{
    unsigned char * rc;
    unsigned char * ptr;
    unsigned char * low;
    // Check for supported base.
    if ( base < 2 || base > 36 )
    {
        *str = '\0';
        return str;
    }
    rc = ptr = str;
    // Set '-' for negative decimals.
    if ( value < 0 && base == 10 )
    {
        *ptr++ = '-';
    }
    // Remember where the numbers start.
    low = ptr;
    // The actual conversion.
    do
    {
        // Modulo is negative for negative value. This trick makes abs() unnecessary.
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while ( value );
    // Terminating the string.
    *ptr-- = '\0';
    // Invert the numbers.
    while ( low < ptr )
    {
        unsigned char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
    return rc;
}

strlen(const char *str) //BSD implementation
{
        const char *s;

        for (s = str; *s; ++s)
                ;
        return (s - str);
}

println(unsigned char *text){
  int i, l;
  for (i = 0; i < strlen(text); i++)
    {
      vga_buffer[xPos] = vga_entry(text[i], BRIGHT_GREEN, BLACK);
      xPos++;
    }
  xPos += ( MaxXInitial - strlen(text));
  yPos++;
}

void printlnVGA(unsigned char *msg){

  unsigned int i=0;
  while(msg[i] != NULL  && msg[i] !='0x00'){
    vga_buffer[xPos] = vga_entry(msg[i], BRIGHT_GREEN, BLACK);
    i++;
    xPos++;
  }
  xPos += ( MaxXInitial - i);
  yPos++;
}


void printchar(unsigned char msg){

  unsigned int i=0;
    vga_buffer[xPos] = vga_entry(msg, BRIGHT_GREEN, BLACK);
    i++;
    xPos++;
    
}
void println_serious_error(unsigned char *msg){

  unsigned int i=0;
  while(msg[i] != NULL  && msg[i] !='0x00'){
    vga_buffer[xPos] = vga_entry(msg[i], BRIGHT_RED, BLACK);
    i++;
    xPos++;
  }
  xPos += ( MaxXInitial - i);
  yPos++;
  halt_cpu();
}


void printchVGA(unsigned char *msg){

  unsigned int i=0;
  while(msg[i] != NULL && msg[i] !='00'){
    vga_buffer[xPos] = vga_entry(msg[i], BRIGHT_GREEN, BLACK);
    i++;
    xPos++;
  }
  xPos += ( i);
  yPos++;
}

void writechar(unsigned char c, unsigned char forecolour, unsigned char backcolour, int x, int y)
{
     uint16 attrib = (backcolour << 4) | (forecolour & 0x0F);
     volatile uint16 * where;
     where = (volatile uint16 *)VGA_ADDRESS + (y * 80 + x) ;
     *where = c | (attrib << 8);
}

void writeline(unsigned char *msg)
{
  int x=0;
  int y=1;
     while(msg[x] != NULL && msg[x] !='00'){
       writechar(&msg[x], BRIGHT_GREEN, BLACK, x, y);
       ++x;
     }
}
