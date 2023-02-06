#include <gfx/vga.h>
#include <gfx/vga_colors.h>

#include <dev/serial.h>
#include <dev/interrupts.h>
#include <dev/cpuid.h>
#include <dev/pic.h>
#include <dev/keyboard.h>

#include <sys/heap.h>
#include <sys/syscalls.h>
#include <sys/program.h>
#include <sys/scheduler.h>

#include <lib/utils.h>

inline static void test_cpu()
{
    vga_printf("\nQuering CPU info\n\n");

    char vendor_id[13];
    cpuid_model(vendor_id);

    vga_printf("CPU Vendor: %s\n", vendor_id);
    vga_printf("ACPI = %b\n", cpuid_check_apic());
}

inline static void test_scheduler()
{
    size_t program_data_1[] = {
        PROGRAM_START,
        PROGRAM_LOAD_MEMORY_BASE, REGISTER_B,

        // 3
        PROGRAM_DEREF, REGISTER_C, REGISTER_B,
        PROGRAM_SYSCALL, SYSCALL_PUTCHAR, REGISTER_C,
        
        PROGRAM_TEST_IMMEDIATE, REGISTER_C, 0,
        PROGRAM_JUMP_IF, 19,
        PROGRAM_INC, REGISTER_B, 8,

        PROGRAM_JUMP, 3,

        // 19
        PROGRAM_NOOP,
        PROGRAM_END,
        'H','e','l','l','o',' ','W','o','r','l','d','\n',0,
    };
    
    Program p1 = {
        .program_data=program_data_1,
        .program_capacity=ARRAY_LENGTH(program_data_1),
    };
    program_init(&p1);
    
    size_t *program_data_2 = (size_t*)alloc(100);
    program_data_2[0]=PROGRAM_START; 
    program_data_2[1]=PROGRAM_SYSCALL_IMMEDIATE; 
    program_data_2[2]=SYSCALL_PUTCHAR; 
    program_data_2[3]='X'; 
    program_data_2[4]=PROGRAM_END;

    // NOTE: using heap allocation
    Program p2 = {
        .program_data=program_data_2,
        .program_capacity=100,
    };
    program_init(&p2);
    
    scheduler_addtask(&p1);
    scheduler_addtask(&p2);
    
    scheduler_start();
}

void kernel_main()
{
    vga_setcolor(VGA_COLOR_MAGENTA);
    vga_printf("Welcome to lavender %c\n", 3);
    vga_restore_color();
    test_scheduler();
    // test_cpu();
    // test_alloc();
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
