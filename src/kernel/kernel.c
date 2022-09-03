#include <utils.c>
#include <print.c>

#include "../gfx/vga.c"

#include "../interrupts/idt.c"
#include "../interrupts/syscall.c"

void test_exceptions()
{
    int a = 1 / 0;
}

void test_syscall()
{
    kernel_syscall(SYSCALL_01);
}

void kernel_main()
{
    vga_setcolor(VGA_COLOR_GREEN);
    printf("[OK] Printf Test = %d\n", 0xB);

    test_exceptions();
    test_syscall();
}

extern void _start()
{
    vga_init();
    idt_init();

    kernel_main();
}
