#include "vgaTextUtility.h"

static volatile unsigned int xPos = 0;
static volatile unsigned int yPos = 0;
static volatile unsigned int single_chars = 0;

static uint16 *vga_buffer;
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
  for (i = 0; i < BUFSIZE; i++)
  {
    (*buffer)[i] = vga_entry(NULL, fore_color, back_color);
  }
  return;
}

//initialize vga buffer
void init_vga(uint8 fore_color, uint8 back_color)
{
  xPos = 0;
  yPos = 0;
  vga_buffer = (uint16 *)(VGA_ADDRESS);                  //point vga_buffer pointer to VGA_ADDRESS
  clear_vga_buffer(&vga_buffer, fore_color, back_color); //clear buffer
  return;
}


void clear()
{
  for (int i = 0; i < xPos; i++)
  {
    vga_buffer[i] = 0;
  }
  //clear_vga_buffer(&vga_buffer, fore_color, back_color);  //clear buffer

  xPos = 0;
  yPos = 0;
  single_chars = 0;
  return;
}


void printlnVGA(unsigned char *msg)
{
  unsigned int i = 0;

  while (msg[i] != NULL && msg[i] != '0x00' && msg[i] != '\n')
  {
    printchar(msg[i]);
    i++;
  }
   printchar('\n');

  return;
}

void print_int_as_char(long i, int base)
{

  unsigned char *str;
  str = int_to_char(i, str, base);
  printlnVGA(str);

  return;
}

void printchar(unsigned char *msg)
{
  if (msg != NULL && msg != '0x00'  && msg != '\b' && msg != '\n' )
  {
    vga_buffer[xPos] = vga_entry(msg, BRIGHT_GREEN, BLACK);
  }

  if (msg == '\n') {
    xPos += (MaxXInitial - single_chars);
    single_chars = 0;
  } else if(msg == '\b'){ //backspace
    xPos-=1;
    single_chars--;
    vga_buffer[xPos] = 0x00;
  } else{
    xPos++;
    single_chars++;
  }
  return;
}

void println_serious_error(unsigned char *msg)
{

  unsigned int i = 0;
  while (msg[i] != NULL && msg[i] != '0x00')
  {
    vga_buffer[xPos] = vga_entry(msg[i], BRIGHT_RED, BLACK);
    i++;
    xPos++;
  }
  xPos += (MaxXInitial - i);
  yPos++;
  halt_cpu();
}


void print_msg(unsigned char *msg)
{
    unsigned int i = 0;
  while (msg[i] != NULL && msg[i] != '0x00'  && msg[i] != '\b')
  {
    vga_buffer[xPos++] = vga_entry(msg[i++], BRIGHT_GREEN, BLACK);
    single_chars++;
  }

  return;
}


int strlen(const char *str) //BSD implementation
{
  const char *s;

  for (s = str; *s; ++s)
    ;
  return (s - str);
}

void remove_written_message_before_newline(unsigned char *msg){
  --xPos;
  --single_chars;
    for(int i=0; i<=strlen(msg); i++){
      vga_buffer[xPos--] = 0x00;
        --single_chars;

    }
  return;
}