#include "process.h";
#include "terminal.h";

static int next_pid = 0;
 pv_process *current_proc;
struct pv_process procs[max_procs]; 

extern int foreground_process;

pv_process create_process(void *code){
    pv_process *process = (pv_process*) malloc(sizeof(pv_process), 1);
    process->pid = ++next_pid;
    process->text_address = code;
    process->status = READY;
    process->eip = 0;
    process->esp = 0;
    process->ebp = 0;

    return *process; 
}

void proc_init(){
    start_terminal_proc();
    void (*r_input)(char) = receive_input;
    void (*dmp)(char) = sample_command;

    foreground_process =  r_input;

    pv_process terminal = create_process(r_input);
    pv_process dample = create_process(dmp);

    procs[0] = terminal;
    //procs[1] = dample;
    process_manager();

}

void process_manager(){

    while(1){
    for(int i = 0; i<2; ++i){
        // run_process(&procs[i], 0x0);

    }


        //TODO

    }
}

//TODO: Run via the scheduler
void run_process(struct pv_process *proc, char args[]){

    if(current_proc){
        current_proc->status = WAITING; 
    }

    current_proc = proc;
    proc->status = RUNNING;

    typedef void function();

    __asm__ __volatile__   ("pusha\n\t");

    function *fun = (function *) proc->text_address;

    fun(args);
    //current_proc = 0x0;
    __asm__ __volatile__   ("popa\n\t");

    return;

}



//Calls a function pointer with an array of arguments
void run_foreground_process(char * args)
{
    if (foreground_process != 0){
        pv_process proc = create_process((void*)foreground_process);
        run_process(&proc, args);
    }
    return;
}



void force_terminate(){
    
    current_proc = 0x0;
    __asm__ __volatile__   ("popa\n\t");

    return;
}