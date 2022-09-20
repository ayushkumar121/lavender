#include <gfx/vga.h>
#include <gfx/vga_colors.h>

#include <dev/serial.h>
#include <dev/interrupts.h>
#include <dev/cpuid.h>
#include <dev/pic.h>
#include <dev/keyboard.h>

#include <sys/syscalls.h>

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

static void test_cpu()
{
    char vendor_id[13];
    cpuid_model(vendor_id);

    vga_printf("CPU Vendor: %s\n", vendor_id);
    vga_printf("ACPI = %b\n", cpuid_check_apic());
}

void kernel_main()
{
    test_vga();
    test_serial();
    test_exceptions();
    test_sycall();
    test_cpu();

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
