#include "types.h"

#include "utils.c"
#include "vga.c"
#include "idt.c"

void kernel_main()
{
    vga_puts( "[OK] ", VGA_COLOR_GREEN);
    vga_puts( "TTY Test\n", VGA_COLOR_DARK_GREY);

    // Testing interrupts
    int a = 1 / 0;
    vga_puts( "[OK] ", VGA_COLOR_GREEN);
    vga_puts( "Interrupts\n", VGA_COLOR_DARK_GREY);
}

extern void _start()
{
    vga_init();
    idt_init();

    kernel_main();
}
