#include <gfx/vga.h>
#include <gfx/vga_colors.h>

#include <dev/serial.h>
#include <dev/interrupts.h>
#include <dev/cpuid.h>
#include <dev/pic.h>
#include <dev/keyboard.h>

#include <sys/syscalls.h>
#include <sys/heap.h>

#include <lib/utils.h>

inline static void test_serial()
{
    serial_printf(COM1, "Testing serial: %d\n", 101);
}

inline static void test_exceptions()
{
    vga_printf("\nTesting Exceptions\n\n");

    int c = 1 / 0;
    vga_printf("Returned from exception\n");
}

inline static void test_sycall()
{
    syscall(SYSCALL_01);
    syscall(SYSCALL_02);
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

void kernel_main()
{
    // test_serial();
    // test_exceptions();
    // test_sycall();
    // test_cpu();
    test_alloc();
    test_keyboard();
}

void _start()
{
    serial_init(COM1);
    interrupts_init();
    // Load drivers that adds their own interrupt handler
    {
        pic_init();
        keyboard_init();
    }
    interrupts_load();
    alloc_init();

    kernel_main();
}
