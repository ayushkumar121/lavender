#include "../lib/utils.c"
#include "../lib/vga.c"
#include "../interrupts/idt.c"

void kernel_main()
{
    vga_puts( "[OK] ", VGA_COLOR_GREEN);
    vga_puts( "TTY Test\n", VGA_COLOR_DARK_GREY);

    // Testing interrupts
    int a = 1 / 0;
    vga_puts( "[OK] ", VGA_COLOR_GREEN);
    vga_puts( "Handling exceptions", VGA_COLOR_DARK_GREY);
}

extern void _start()
{
    vga_init();
    idt_init();

    kernel_main();
}
