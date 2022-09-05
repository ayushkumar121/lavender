#include <gfx/vga.h>
#include <gfx/vga_colors.h>
// #include <lib/utils.h>
// #include <lib/print.h>
// #include <dev/serial.h>
// #include <lib/types.h>

// #include <sys/syscalls.h>
// #include <interrupts/idt.h>

void test_vga()
{
    // vga_setcolor(VGA_COLOR_BROWN);
    // vga_puts("Hello world");
    // setcolor(VGA_COLOR_GREEN);
    // printf("[OK] Printf Test = %d\n", 0xB);
}

// // void test_exceptions()
// // {
// //     int a = 1 / 0;
// // }

// void test_syscalls()
// {
//     syscall(SYSCALL_01);
//     syscall(SYSCALL_02);
// }

// void test_serial()
// {
//     serial_printf(COM1, "Hello serial %d\n", 2);
// }

void kernel_main()
{
    test_vga();
    // test_exceptions();
    // test_syscalls();
    // test_serial();
}


extern void _start()
{
    vga_init();
    // idt_init();
    // serial_init(COM1);

    kernel_main();
}
