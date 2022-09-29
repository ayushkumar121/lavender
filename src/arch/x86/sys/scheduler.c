#include <sys/scheduler.h>

#include <dev/pic.h>
#include <dev/interrupts.h>

#include <gfx/vga.h>

#define SCHEDULER_INT_INDEX 0x81

typedef struct
{
    // Task task;
    InterruptFrame frame;
    /* any other options */
} TaskState;

static TaskState tasks[256];
static int pid_count = 0;
static int current_pid = 0;

void save_state(InterruptFrame *frame, int pid)
{
    vga_printf("Saving state for %d\n", pid);
    tasks[pid].frame.cs = frame->cs;
    tasks[pid].frame.ip = frame->ip;
    tasks[pid].frame.sp = frame->sp;
    tasks[pid].frame.ss = frame->ss;
    tasks[pid].frame.flags = frame->flags;
}

void load_state(InterruptFrame *frame, int pid)
{
    vga_printf("Loading state for %d\n", pid);
    frame->cs = tasks[pid].frame.cs;
    frame->ip = tasks[pid].frame.ip;
    frame->sp = tasks[pid].frame.sp;
    frame->ss = tasks[pid].frame.ss;
    frame->flags = tasks[pid].frame.flags;
}

TaskState copy_state(InterruptFrame *frame)
{
    TaskState state;
    state.frame.cs = frame->cs;
    state.frame.ip = frame->ip;
    state.frame.sp = frame->sp;
    state.frame.ss = frame->ss;
    state.frame.flags = frame->flags;

    return state;
}

__attribute__((interrupt)) static void tick_handler(InterruptFrame *frame)
{
    // // /* Save current state */
    // save_state(frame, current_pid);

    // /* Advance the pid */
    // current_pid++;

    // /* Warp around to first task */
    // if (current_pid >= pid_count)
    // {
    //     current_pid = 0;
    // }

    // /* Load next state this should be enough for the jump */
    // load_state(frame, current_pid);

    /* Jump to the next task */
    pic_eoi(TIMER_INT_INDEX);
}

__attribute__((interrupt)) static void schedule_handler(InterruptFrame *frame)
{
    current_pid = pid_count++;

    TaskState state = copy_state(frame);
    state.frame.ip = ++(frame->ip);
    save_state(&state.frame, current_pid);
}

void scheduler_init()
{
    interrupts_add_handler(TIMER_INT_INDEX, tick_handler, INT_GATE);
    interrupts_add_handler(SCHEDULER_INT_INDEX, schedule_handler, INT_GATE);
}

void scheduler_taskwait()
{
    for (size_t i = 0; i < 200000; i++)
    {
        __asm__("nop");
    }
}

int scheduler_addtask(Task task)
{
    __asm__ volatile("int 0x81");
    task();

    return pid_count + 1;
}