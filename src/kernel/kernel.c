#include <gfx/vga_colors.h>

void kernel_main()
{
    char *mem = 0xb8000;
    *mem  = 'A';
    *(mem+1)  = VGA_COLOR_BLUE << 4 | VGA_COLOR_CYAN ;
}


extern void _start()
{
    kernel_main();
}
