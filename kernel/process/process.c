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

//TODO: Run via the scheduler
//TODO: Push current cpu stacks
void run_process(struct pv_process *proc, char args[]){

    if(current_proc){
        current_proc->status = WAITING; 
    }
    current_proc = proc;

    current_proc->status = RUNNING;

    typedef void func();
    func *f = (func *) current_proc->image.text_address;
    f(args);
    return;

}