#include "cpu.h"


enum process_status{
    BLOCKED,
    READY,
    RUNNING
}; //POSIX-compliant

enum thread_status{
    THREAD_BLOCKED,
    THREAD_READY,
    THREAD_RUNNING,
    THREAD_DELAYED,
    THREAD_WAITING,
    THREAD_EXITING
};


typedef  struct pv_thread {
    uint32 thread_id; //offset in pv_process.threads!
	struct pushed_values registers;
    uint32 parent_process_id;
    enum thread_status status;
	struct pv_thread *previous_thread; 
	struct pv_thread *next_thread; 

};


typedef  struct pv_process {
    uint32 id;
    uint32 total_threads;
    enum process_status status;
	struct pv_thread *threads; 
    //TODO: add files,
    uint32 parent_process_id;
    struct pv_process *child_processes; 
};


struct sys_processes{
    struct pv_process *processes;
};



struct pv_process *add_process(void *object_file);
void *remove_process(struct pv_process *pv_process);

struct pv_process create_thread(struct pv_process *pv_process);
void remove_thread(struct pv_thread *pv_thread);
int thread_join(struct pv_thread *thread);
int thread_yield(struct pv_thread *release_cpu_for_this_thread);
