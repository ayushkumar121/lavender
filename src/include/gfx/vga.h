#pragma once

#include <lib/utils.h>

#define ROWS 25
#define COLS 80

typedef struct
{
    char ascii_character;
    char color_code;
} VgaScreenChar;

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

    int initialized;
} VgaWriter;

void vga_init();
void vga_newline();
void vga_putchar(char ch);
void vga_setcolor(char color_code);
void vga_restore_color();
void vga_puts(char *str);