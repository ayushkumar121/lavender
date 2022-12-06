#include <sys/scheduler.h>

#include <dev/pic.h>
#include <dev/interrupts.h>
#include <dev/serial.h>

#include <gfx/vga.h>
#include <lib/types.h>
#include <lib/mutex.h>

#define SCHEDULER_INT_INDEX 0x81

static Program processes[256];
static int pid_count = 0;

typedef struct
{
    int current_pid;
    uint32_t mutex_lock;
} ProcessState;

static ProcessState state = {
   .current_pid = -1,
   .mutex_lock = MUTEX_UNLOCKED,
};

void dirty_sleep()
{
    for(int i=0; i<10000000;i++) 
    {
        __asm("nop;");
    }
}
__attribute__((interrupt)) static void tick_handler(InterruptFrame *frame)
{
    if(pid_count > 0)
    {
        state.current_pid++;
        
        if (state.current_pid >= pid_count) 
        {
            state.current_pid= 0;
        } 
    } 
    
    pic_eoi(TIMER_INT_INDEX);
}

void scheduler_init()
{
    interrupts_add_handler(TIMER_INT_INDEX, tick_handler, INT_GATE);
}

int scheduler_addtask(Program program)
{
    processes[pid_count++] = program;
    return pid_count;
}


void scheduler_start()
{
    while(1) 
    {
        spin_lock(&state.mutex_lock);
        if (state.current_pid > -1)
        {
            // we're locking on state so we don't interrupt don't update
            // current pid mid running
            Program *p = &processes[state.current_pid];
        
            // We can run program
            if (p->ip < p->len)
            {
                vga_printf("Running process: %d\n", state.current_pid);
                p->ip++;
            }
            
        }
        
        dirty_sleep();
        spin_unlock(&state.mutex_lock);
    }
}