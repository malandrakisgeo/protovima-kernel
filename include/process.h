#pragma once

#include "cpu.h"
#include "commands.h"


enum process_status{
    BLOCKED,
    READY, //The process is waiting to be assigned to a processor.
    RUNNING,
    INTERRUPTED,
    WAITING, //The process is waiting for some event to occur,
    COMPLETED
}; //POSIX-compliant

/*typedef struct exec_image {
	void* text_address; //i.e. a function pointer
	unsigned int esp, ebp;       // Stack and base pointers.
    unsigned int eip;            // Instruction pointer.
}exec_image;*/



typedef struct pv_process{
    int pid;
    int prio;
    enum process_status status;
   // struct cpu_state registers; //TODO: choose between cpu_state and pushed_values
    long int creation_timestamp;
    long text_address; // Function pointer to code. Was void.
    unsigned int esp, ebp;       // Stack and base pointers.
    unsigned int eip;            // Instruction pointer.
    unsigned long max_address;
    char* args;
}pv_process;

typedef struct running_proc_list{
   struct pv_process *process;
   struct running_proc_list *next;
    struct running_proc_list *previous;
}running_proc_list;





struct running_proc_list create_process(long *code, char *args);
void run_process(struct pv_process *pv_process, char args[]);

void force_task_switch();
