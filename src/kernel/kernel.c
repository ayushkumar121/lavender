#include <gfx/vga.h>
#include <gfx/vga_colors.h>

#include <dev/serial.h>

void kernel_main()
{
    vga_printf("HIIIIIIIIIIIIIIIII");
    serial_printf(COM1, "Hello world\n");
}

extern void _start()
{
    vga_init();
    serial_init(COM1);
    kernel_main();
}
