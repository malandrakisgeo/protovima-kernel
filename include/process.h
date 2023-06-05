#pragma once

#include "cpu.h"
#include "commands.h"


enum process_status{
    BLOCKED,
    READY, //The process is waiting to be assigned to a processor.
    RUNNING,
    WAITING //The process is waiting for some event to occur
}; //POSIX-compliant

typedef struct exec_image {
	 void* text_address; //i.e. a function pointer
	//unsigned long heap; //future use
	//unsigned long stack;
}exec_image;



typedef struct pv_process{
    int pid;
    enum process_status status;
    //TODO: add files, threads, etc
    struct cpu_state registers; //TODO: choose between cpu_state and pushed_values
    long int creation_timestamp;
    struct exec_image image;
    struct pv_process *parent_process;
    struct pv_process *child_processes_list; 
}pv_process;

typedef struct child_processes_list{
   pv_process process;
   struct pv_process *next;
}child_processes_list;

struct sys_processes_table{
    struct pv_process *processes;
};



struct pv_process create_process(void *code);
void run_process(struct pv_process *pv_process, char args[]);

