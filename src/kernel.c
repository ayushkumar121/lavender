#include "utils.c"
#include "vga.c"

#include "types.h"

void main()
{
    char buf[100];
    int word_size = sizeof(long long int);
    itoa(word_size, buf);

    struct VgaWriter writer;

    vga_init(&writer);
    vga_puts(&writer, buf, VGA_COLOR_WHITE);
    vga_puts(&writer, "[OK] ", VGA_COLOR_GREEN);
    vga_puts(&writer, "TTY Test\n", VGA_COLOR_DARK_GREY);

    // Testing inline assembly
    // asm("mov al, 'A';"
    //     "mov ah, 0x0f;"
    //     "mov [0xb8000], ax;");

    // Testing interrupts
    // int a = 1 / 0;

    // vga_puts(&writer, "[OK] ", VGA_COLOR_GREEN);
    // vga_puts(&writer, "Divide by zero handled\n", VGA_COLOR_DARK_GREY);
}

extern void _start()
{
    main();
}
