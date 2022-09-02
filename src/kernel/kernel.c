#include "../lib/utils.c"
#include "../lib/vga.c"

#include "../interrupts/idt.c"
#include "../interrupts/syscall.c"

void kernel_main()
{
    vga_setcolor(VGA_COLOR_GREEN);
    vga_puts( "[OK] ");
    vga_puts( "TTY Test\n");

    // Testing interrupts
    int a = 1 / 0;

    vga_puts( "[OK] ");
    vga_puts( "Handling exceptions\n");

    syscall(SYSCALL_01);

    vga_puts( "[OK] ");
    vga_puts( "Handling syscalls\n");
}

extern void _start()
{
    vga_init();
    idt_init();

    kernel_main();
}
