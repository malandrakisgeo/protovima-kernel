#include "kernel.h"
#include "timer.h"
#include "pic-8259.h"
#include "inline-assembly.h"


 void timer_handler(){
  //printlnVGA("timer!");
   //TODO: task switch!
   return;
}

void init_timer(int frequency)
{
   // The value we send to the PIT is the value to divide it's input clock
   // (1193180 Hz) by, to get our required frequency. Important to note is
   // that the divisor must be small enough to fit into 16-bits.
    int divisor = 1193180 / frequency;

   // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
   unsigned char l = (unsigned char) (divisor & 0xFF);
   unsigned char h = (unsigned char) ((divisor>>8) & 0xFF);

   // Send the command byte and the frequency divisor.
   outb(0x43, 0x36);
   outb(0x40, l);
   outb(0x40, h);

   return;
} 