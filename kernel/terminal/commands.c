/*
COMMAND LIST: Time: Prints current time
 Halt: Halts the CPU
  Say: Prints whatever comes afterwards.
    malloc: Allocates memory
    dealloc: Deallocates memory

*/
#include "inline-assembly.h"

void cpu_info(){     //WIP
    //WIP
        //TODO: fix this
    int a, b, c, d;
    a = 0x1;
    int ret;
        __asm__ __volatile__   (
                "pusha\n\t"

    "mov %%eax, %0\n\t"
    "cpuid\n\t"
    "mov %0, %%eax\n\t"
    "mov %1, %%ebx\n\t"
    "mov %2, %%ecx\n\t"
    "mov %3, %%edx\n\t"
                    "popa\n\t"

    : "=r" (a), "=r" (b), "=r" (c), "=r" (d)
    : "0" (a)
    );
        print_int_as_char(a, 16);
        print_int_as_char(b, 16);

    print_int_as_char(c, 16);
        print_int_as_char(d, 16);



     return;
}


void halt(){
    while(1){
        ;;
    }
}