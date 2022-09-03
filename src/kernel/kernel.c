
#include <gfx/vga.h>
#include <lib/utils.h>
#include <lib/print.h>
#include <sys/syscalls.h>
#include <dev/serial.h>

// Should only be include by the kernel
#include <interrupts/idt.h>

void test_vga()
{
    setcolor(VGA_COLOR_GREEN);
    printf("[OK] Printf Test = %d\n", 0xB);
}

void test_exceptions()
{
    int a = 1 / 0;
}

void test_syscalls()
{
    syscall(SYSCALL_01);
    syscall(SYSCALL_02);
}

void test_serial()
{
    serial_printf(COM1, "Hello serial");
}

void kernel_main()
{
    test_vga();
    test_exceptions();
    test_syscalls();
    test_serial();
}

extern void _start()
{
    vga_init();
    idt_init();
    serial_init(COM1);

    kernel_main();
}
