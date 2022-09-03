#pragma once

#include <lib/utils.h>

#define ROWS 25
#define COLS 80

typedef struct
{
    int col;
    int row;
    char *buf;
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