#include <gfx/vga.h>
#include <gfx/vga_colors.h>

#include <dev/serial.h>
#include <dev/interrupts.h>
#include <dev/pic.h>
#include <dev/keyboard.h>

#include <sys/heap.h>
#include <sys/syscalls.h>
#include <sys/program.h>
#include <sys/scheduler.h>

#include <lib/utils.h>

inline static void test_scheduler()
{
    size_t pong_program[100] = {
        PROGRAM_START,
        PROGRAM_LOAD, REGISTER_A, CHANNEL_0,       // A=CHANNEL_0
        PROGRAM_LOAD_MEMORY_BASE, REGISTER_B,     // B=&static_memory
        PROGRAM_SUBSCRIBE, REGISTER_A, REGISTER_B,
        
        PROGRAM_NOOP, // Infinite Loop start,

        // Printing the received value
        PROGRAM_LOAD_MEMORY_BASE, REGISTER_B,     // B=&static_memory
        
        PROGRAM_DEREF, REGISTER_C, REGISTER_B,   // C=*B
        PROGRAM_TEST_IMMEDIATE, REGISTER_C, 0,   // flag=(C==0)
        PROGRAM_JUMP_IF, 9,                      // jump 9th inst if flag is set

        PROGRAM_SYSCALL, SYSCALL_PUTCHAR, REGISTER_C,
        PROGRAM_INC, REGISTER_B, 8,
        PROGRAM_JUMP, 12,

        PROGRAM_END,
        0,
    };
   
    Program pong = {
        .program_data=pong_program,
        .program_capacity=ARRAY_LENGTH(pong_program),
    };
    program_init(&pong);
    scheduler_addtask(&pong);
 
    size_t ping_program[100] = {
        PROGRAM_START,
        PROGRAM_LOAD, REGISTER_A, CHANNEL_0,      // A=101
        PROGRAM_LOAD_MEMORY_BASE, REGISTER_B,     // B=&static_memory
	      PROGRAM_PUBLISH, REGISTER_A, REGISTER_B,
	      PROGRAM_END,
        'H', 'e', 'l', 'l', 'o', '\n', 0,
    };

    Program ping = {
        .program_data=ping_program,
        .program_capacity=ARRAY_LENGTH(ping_program),
    };
    program_init(&ping);
    scheduler_addtask(&ping);

    scheduler_start();
}

void kernel_main()
{
    vga_setcolor(VGA_COLOR_MAGENTA);
    vga_printf("Welcome to lavender %c\n", 3);
    vga_restore_color();
    test_scheduler();
}

void _start()
{
    serial_init(COM1);
    alloc_init();
    interrupts_init();
    pic_init();
    keyboard_init();
    scheduler_init();
    interrupts_load();
    kernel_main();
}
