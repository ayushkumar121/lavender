
#include <gfx/vga.h>
#include <lib/utils.h>
#include <lib/print.h>
#include <sys/syscalls.h>

// Should only be include by the kernel
#include <interrupts/idt.h>

void test_exceptions()
{
    int a = 1 / 0;
}

void test_syscalls()
{
    syscall(SYSCALL_01);
    syscall(SYSCALL_02);
}

void kernel_main()
{
    setcolor(VGA_COLOR_GREEN);
    printf("[OK] Printf Test = %d\n", 0xB);

    test_exceptions();
    test_syscalls();

    // init_serial();
    // write_serial('A');
}

extern void _start()
{
    vga_init();
    idt_init();

    kernel_main();
}
