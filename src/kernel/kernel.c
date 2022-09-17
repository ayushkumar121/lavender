#include <gfx/vga.h>
#include <gfx/vga_colors.h>

#include <dev/serial.h>
#include <dev/interrupts.h>

#include <sys/syscalls.h>

inline static void test_vga()
{
    vga_printf("Testing VGA text mode :(%d) \n", 10);
}

inline static void test_serial()
{
    serial_printf(COM1, "Testing serial\n");
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

void kernel_main()
{
    test_vga();
    test_serial();
    test_exceptions();
    test_sycall();
}

extern void _start()
{
    vga_init();
    serial_init(COM1);
    interrupts_init();

    kernel_main();
}
