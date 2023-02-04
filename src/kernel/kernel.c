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

inline static void test_serial()
{
    serial_printf(COM1, "Testing serial: %d\n", 101);
}

inline static void test_cpu()
{
    vga_printf("\nQuering CPU info\n\n");

    char vendor_id[13];
    cpuid_model(vendor_id);

    vga_printf("CPU Vendor: %s\n", vendor_id);
    vga_printf("ACPI = %b\n", cpuid_check_apic());
}

inline static void test_alloc()
{
    vga_printf("\nTesting Temp Allocation\n\n");

    char *a = (char *)temp_alloc(10);
    memcpy(a, "Ayush", 6);
    vga_printf("Allocating \"%s\"\n", a);

    char *b = (char *)temp_alloc(1 << 20);
    vga_printf("Null pointer on large allocation %x\n", b);

    char *c = (char *)temp_alloc(21);
    char *d = (char *)temp_alloc(8);
    vga_printf("Alignment check %x, %x\n", c, d);

    temp_rollback(a);

    a = (char *)temp_alloc(8);
    vga_printf("Heap start after free %x\n", a);
    temp_rollback(a);

    vga_printf("\nTesting Linked List Allocation\n");

    a = (char *)alloc(100);
    free(a, 100);
    b = (char *)alloc(150);
    c = (char *)alloc(20);
    free(b, 150);
    d = (char *)alloc(10);

    vga_printf("\n A=%x\n B=%x\n C=%x\n D=%x\n", a, b, c, d);
}

inline static void test_keyboard()
{
    while (true)
    {
        char ch = keyboard_getch(KEY_EVENT_PRESSED);
        vga_printf("%c", ch);
    }
}

static void test_scheduler()
{
    size_t program_data_1[] = {
        PROGRAM_START,
        PROGRAM_LOAD_IMMEDIATE, REGISTER_A, 0,
        PROGRAM_LOAD_IMMEDIATE, REGISTER_D, 5,
        
        PROGRAM_LOAD_MEMORY_BASE, REGISTER_B,

        // 9
        PROGRAM_DEREF, REGISTER_C, REGISTER_B,
        PROGRAM_SYSCALL, SYSCALL_PUTCHAR, REGISTER_C,
        
        PROGRAM_TEST, REGISTER_A, REGISTER_D,
        PROGRAM_JUMP_IF, 28,
        PROGRAM_INC, REGISTER_A, 1,
        PROGRAM_INC, REGISTER_B, 8,

        PROGRAM_JUMP, 9,

        // 28
        PROGRAM_NOOP,
        PROGRAM_END,
        'H','e','l','l','o','\n',
    };
    
    Program p1 = {
        .program_data=program_data_1,
        .program_capacity=ARRAY_LENGTH(program_data_1),
    };
    program_init(&p1);
    
    size_t program_data_2[] = {
        PROGRAM_START,
        PROGRAM_LOAD_IMMEDIATE, REGISTER_A, 1,
        PROGRAM_LOAD_IMMEDIATE, REGISTER_B, 2,
        
        PROGRAM_ADD, REGISTER_A, REGISTER_B,
        PROGRAM_PRINT, REGISTER_A,
        
        PROGRAM_END,
    };
    
    Program p2 = {
        .program_data=program_data_2,
        .program_capacity=ARRAY_LENGTH(program_data_2),
    };
    program_init(&p2);
    
    scheduler_addtask(&p1);
    scheduler_addtask(&p2);
    
    scheduler_start();
}

void kernel_main()
{
    test_scheduler();    
//     test_serial();
//     test_cpu();
//     test_alloc();
//     test_keyboard();
//     test_scheduler();
//     scheduler_start();
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
