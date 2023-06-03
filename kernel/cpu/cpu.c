#include "cpu.h"


inline void disable_interrupts()
{
	__asm__ volatile ("cli");
}

void halt_cpu(){
    disable_interrupts();
        for(;;) {}  
}