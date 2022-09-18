#include <gfx/vga.h>
#include <gfx/vga_colors.h>

#include <dev/serial.h>
#include <dev/interrupts.h>
#include <dev/cpuid.h>
#include <dev/pic.h>

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
}

__attribute__((interrupt)) void timer_handler(InterruptFrame *frame)
{
    vga_printf(".");
    pic_eoi();
}

extern void _start()
{
    vga_init();
    serial_init(COM1);
    interrupts_init();
    pic_init();

    // Register interupts
    interrupts_add_handler(PIC_1_OFFSET, timer_handler, INT_GATE_USER);

    interrupts_load();

    kernel_main();
}
