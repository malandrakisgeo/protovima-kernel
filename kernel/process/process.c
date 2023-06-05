#include "process.h";

static int next_pid = 0;
 pv_process *current_proc;

pv_process create_process(void *code){

    pv_process process = { 
        .pid = next_pid++,
        .image.text_address = code,
        .creation_timestamp = 1, //TODO: Give a proper timestamp
        .status = READY
        };


    return process;
}

void proc_init(){
        //TODO
}

void process_manager(){
    while(1){
        //TODO

    }
}

//TODO: Run via the scheduler
void run_process(struct pv_process *proc, char args[]){

    if(current_proc){
        current_proc->status = WAITING; 
    }
    current_proc = proc;

    current_proc->status = RUNNING;

    typedef void function();

    __asm__ __volatile__   ("pusha\n\t");

    function *fun = (function *) current_proc->image.text_address;
    fun(args);
    current_proc = 0x0;
    __asm__ __volatile__   ("popa\n\t");

    return;

}

void force_terminate(){
    
    current_proc = 0x0;
    __asm__ __volatile__   ("popa\n\t");

    return;
}