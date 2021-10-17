#include "kernel.h"

struct cpu_state {
    // Von Hand gesicherte Register
    uint32   eax;
    uint32   ebx;
    uint32   ecx;
    uint32   edx;
    uint32   esi;
    uint32   edi;
    uint32   ebp;

    uint32   intr;
    uint32   error;

    // Von der CPU gesichert
    uint32   eip;
    uint32   cs;
    uint32   eflags;
    uint32   esp;
    uint32   ss;
};

struct pushed_values {
    uint32 gs, fs, es, ds;      //Pushed the segment registers last
    //  eax, ecx, edx, ebx, esp, ebi, esi, edi
    uint32 edi, esi, ebp, esp, ebx, edx, ecx, eax;  //Pushed by 'pushad'
    	uint32 int_no, err_code;    //Our 'push byte #' and ecodes do this
	uint32 eip, cs, eflags, useresp, ss;   //Pushed by the processor automatically

    /*uint32   eax;
    uint32   ecx;
    uint32   edx;
    uint32   ebp;
    uint32   esp;
    uint32   ebi;
    uint32   esi;
    uint32   edi;*/

};

void halt_cpu(void);

void disable_interrupts(void);
