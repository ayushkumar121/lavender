#include <gfx/vga.h>
#include <gfx/vga_colors.h>
#include <dev/serial.h>
#include <lib/sstring.h>

#include <stdarg.h>

#define ROWS 25
#define COLS 80

typedef struct
{
    char ascii_character;
    char color_code;
} __attribute__((packed)) VgaScreenChar;

typedef struct
{
    VgaScreenChar chars[ROWS * COLS];
} VgaBuffer;

typedef struct
{
    VgaBuffer *buffer;
    
    int row;
    int col;
    char color;
    char previous_color;
} VgaWriter;

// TODO: add a mutex to writer
static VgaWriter writer;

void vga_init()
{
    writer.buffer = (VgaBuffer*)0xb8000;
    writer.col = 0;
    writer.row = 0;
    writer.color = VGA_COLOR_WHITE;
    writer.previous_color = VGA_COLOR_WHITE;
}

void vga_newline()
{
    writer.col = 0;
    writer.row++;
}

void vga_putchar(char ch)
{
    int k = (writer.row * COLS + writer.col);

    if (ch != '\n')
    {
        writer.buffer->chars[k].ascii_character = ch;
        writer.buffer->chars[k].color_code = writer.color;

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

__attribute__((format(printf, 1, 2))) void vga_printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    SString ss = ss_vprintf(fmt, args);
    vga_puts(ss.data);

    va_end(args);
}