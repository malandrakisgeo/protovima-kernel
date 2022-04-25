#include "cpu.h"


enum process_status{
    BLOCKED,
    READY, //The process is waiting to be assigned to a processor.
    RUNNING,
    WAITING //The process is waiting for some event to occur
}; //POSIX-compliant

enum thread_status{
    THREAD_BLOCKED,
    THREAD_READY,
    THREAD_RUNNING,
    THREAD_DELAYED,
    THREAD_WAITING,
    THREAD_EXITING
};


typedef struct pv_thread {
    uint32 thread_id; //mallon tha einai akeraios pou antistoixei sto n. 1 gia prwto thread, 2 gia 2o k.o.k...
	struct pushed_values registers;
    struct pv_process *parent_process;
    enum thread_status status;
	struct pv_thread *previous_thread; 
	struct pv_thread *next_thread; 
    uint32 program_counter; //address of the next instruction. TODO: Des an to theloume.

}pv_thread;


typedef  struct pv_process {
    uint32 id;
    uint32 total_threads;
    enum process_status status;
	struct pv_thread *threads; //todo: Na deixnei panta to prwto arage? 
    //TODO: add files,
    struct pv_process *parent_process;
    struct pv_process *child_processes; 
}pv_process;


struct sys_processes{
    struct pv_process *processes;
};



struct pv_process *add_process(void *object_file);
void *remove_process(struct pv_process *pv_process);

struct pv_process create_thread(struct pv_process *pv_process);
void remove_thread(struct pv_thread *pv_thread);
int thread_join(struct pv_thread *thread);
int thread_yield(struct pv_thread *release_cpu_for_this_thread);
