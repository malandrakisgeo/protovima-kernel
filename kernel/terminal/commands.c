/*
COMMAND LIST: Time: Prints current time
 Halt: Halts the CPU
  Say: Prints whatever comes afterwards.
    malloc: Allocates memory
    dealloc: Deallocates memory

*/
#include "inline-assembly.h"


void sample_command()
{
    printlnVGA("Sample command ran!");
    return;
}

void dample_command(char *ch)
{
    if (ch[0] != 0)
    {
        printlnVGA("You inserted the arguments: ");
        printlnVGA(ch);
    }
    else
    {
        printlnVGA("No arguments inserted.");
    }

    return;
}

void malloc_command(char *size)
{   
    int rc = 0;
    unsigned i = 1; //0 is a space
    // C guarantees that '0'-'9' have consecutive values
    while (size[i] != 0 && size[i] != '0x00')
    {

        if (size[i] >= 48 && size[i] <= 57)
        {
            rc *= 10;
            rc += (size[i] - 48);
        }
        ++i;
    }

    malloc(rc, 0);

    return;
}

void clear_terminal_command()
{
    clear();
    return;
}


//TODO: Fix manage processing so that calling this from terminal will not halt the system.
void inf_loop()
{
    while(1){

    }
    return;
}


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