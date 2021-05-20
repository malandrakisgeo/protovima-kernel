
int xPos = 0;
int yPos = 0;
#define MaxYInitial 25
#define MaxXInitial 80

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
  uint16 ax = 0;
  uint8 ah = 0, al = 0;

  ah = back_color;
  ah <<= 4;
  ah |= fore_color;
  ax = ah;
  ax <<= 8;
  al = ch;
  ax |= al;

  return ax;
}

//clear video buffer array
void clear_vga_buffer(uint16 **buffer, uint8 fore_color, uint8 back_color)
{
  uint32 i;
  for(i = 0; i < BUFSIZE; i++){
    (*buffer)[i] = vga_entry(NULL, fore_color, back_color);
  }
}



//
void printlnVGA(char *msg){
    //vga_buffer[xPos] = vga_entry('\n', WHITE, BLACK);

  int i=0;
  while(msg[i] != NULL){
    vga_buffer[xPos] = vga_entry(msg[i], WHITE, BLACK);
    i++;
    xPos++;
  }
  xPos += ( MaxXInitial - i);
  yPos++;
}

