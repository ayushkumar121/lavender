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
    size_t ping_program[] = {
        PROGRAM_START,
    	
	PROGRAM_PING,

	// Infinite Loop
	PROGRAM_JUMP, 0,

	PROGRAM_END, 
    };
    
    Program ping = {
        .program_data=ping_program,
        .program_capacity=ARRAY_LENGTH(ping_program),
    };
    program_init(&ping);
    scheduler_addtask(&ping);
    
    size_t pong_program[] = {
	PROGRAM_START,
        
	PROGRAM_PONG,
	
	// Infinite Loop
	PROGRAM_JUMP, 0,

	PROGRAM_END,
    };
   
    Program pong = {
        .program_data=pong_program,
        .program_capacity=100,
    };
    program_init(&pong);
    scheduler_addtask(&pong);
    
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
    
    // Load drivers that adds their own interrupt handler
    pic_init();
    keyboard_init();
    scheduler_init();
    
    interrupts_load();
    kernel_main();
}
