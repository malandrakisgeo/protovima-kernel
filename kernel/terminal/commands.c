/*
COMMAND LIST: Time: Prints current time
 Halt: Halts the CPU
  Say: Prints whatever comes afterwards.
    malloc: Allocates memory
    dealloc: Deallocates memory

*/
#include "inline-assembly.h"
extern volatile long val;

void charprint()
{
    printchar('a');
    return;
}


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


//The purpose with this was to fix process management and interrupts so that calling this via the terminal would not halt the system.
void inf_loop()
{
    printlnVGA("inf_loop began");
    while(1){
        ++val; //Can be tested via the showval-command
    }
    return; //TODO: Add break by escaping
}

//Meant to "debug" the infloop command, by seing if it really is interrupted when running the while(1) that increases val.
void showval(){
    print_int_as_char(val, 10);

    return;
}


void cpu_info(){     //WIP
    //WIP
        //TODO: fix this
    /*int a, b, c, d;
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
*/
     return;
}


void halt(){
    while(1){
        ;;
    }
}