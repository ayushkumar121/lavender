#include <sys/scheduler.h>

#include <dev/pic.h>
#include <dev/interrupts.h>
#include <dev/serial.h>

#include <gfx/vga.h>
#include <lib/types.h>
#include <lib/mutex.h>

#define SCHEDULER_INT_INDEX 0x81
typedef struct
{
    int64_t current_pid;
    uint32_t mutex_lock;
} ProcessState;

static Program *processes[256];
static size_t pid_count = 0;

static ProcessState state = {
   .current_pid = -1,
   .mutex_lock = MUTEX_UNLOCKED,
};

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

void scheduler_addtask(Program *program)
{
    int64_t pid = pid_count++;
    program->pid = pid;
    processes[pid] = program;
}

void scheduler_start()
{
    while(1) 
    {
        spin_lock(&state.mutex_lock);
        if (state.current_pid > -1)
        {
            Program *p = processes[state.current_pid];
            program_step(p);
        }
        // vga_printf(".");
        spin_unlock(&state.mutex_lock);
    }
}
