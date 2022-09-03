#pragma once

#include <utils.c>

#define ROWS 25
#define COLS 80

enum vga_color
{
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

typedef struct
{
    int col;
    int row;
    char *buf;
    char color;
    char previous_color;
    int initialized;
} VgaWriter;

VgaWriter writer;

void vga_init()
{
    writer.buf = (char *)0xb8000;
    writer.col = 0;
    writer.row = 0;
    writer.color = VGA_COLOR_WHITE;
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
        writer.buf[k] = ch;
        writer.buf[k + 1] = writer.color;

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