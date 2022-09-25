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

inline static void test_vga()
{
    vga_printf("VGA text mode: %d\n", 101);
}

inline static void test_serial()
{
    serial_printf(COM1, "Testing serial: %d\n", 101);
}

inline static void test_exceptions()
{
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
    char vendor_id[13];
    cpuid_model(vendor_id);

    vga_printf("CPU Vendor: %s\n", vendor_id);
    vga_printf("ACPI = %b\n", cpuid_check_apic());
}

inline static void test_alloc()
{
    vga_printf("Testing Heap Allocation\n\n");
    
    char *a = (char *)heap_alloc(10);
    memcpy(a, "Ayush", 6);
    vga_printf("[OK] Allocating \"%s\"\n", a);

    char *b = (char *)heap_alloc(1 << 20);
    vga_printf("[OK] Null pointer on large allocation %x\n", b);

    char *c = (char *)heap_alloc(21);
    vga_printf("[OK] %x\n", c);
    heap_free(c);

    char *d = (char *)heap_alloc(8);
    vga_printf("[OK] Alignment check  %x\n", d);
    heap_free(d);
}

void kernel_main()
{
    test_vga();
    test_serial();
    test_exceptions();
    test_sycall();
    test_cpu();
    test_alloc();

    while (true)
    {
        char ch = keyboard_getch(KEY_EVENT_PRESSED);
        vga_printf("%c", ch);
    }
}

void _start()
{
    serial_init(COM1);

    interrupts_init();
    pic_init();
    keyboard_init();
    interrupts_load();

    kernel_main();
}
