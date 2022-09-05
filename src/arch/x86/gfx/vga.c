#include <gfx/vga.h>
#include <gfx/vga_colors.h>

// TODO: add a mutex to writer
VgaWriter writer;

void vga_init()
{
    writer.buffer = (VgaBuffer *)0xb8000;
    writer.col = 0;
    writer.row = 0;
    writer.color = VGA_COLOR_WHITE;
    writer.previous_color = VGA_COLOR_WHITE;
    writer.initialized = true;
}

void vga_newline()
{
    writer.col = 0;
    writer.row++;
}

void vga_putchar(char ch)
{
    if (!writer.initialized)
        return;

    int k = (writer.row * COLS + writer.col) * 2;

    if (ch != '\n')
    {
        writer.buffer->chars[k].ascii_character = ch;
        writer.buffer->chars[k + 1].color_code = writer.color;

        if (writer.col < COLS)
        {
            writer.col++;
        }
        else
        {
            vga_newline();
        }
    }
    else
    {
        vga_newline();
    }
}

void vga_setcolor(char color_code)
{
    writer.previous_color = writer.color;
    writer.color = color_code;
}

void vga_restore_color()
{
    char temp = writer.color;
    writer.color = writer.previous_color;
    writer.previous_color = temp;
}

void vga_puts(char *str)
{
    while (*str)
    {
        vga_putchar(*str++);
    }
}