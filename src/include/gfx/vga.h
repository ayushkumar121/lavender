#pragma once

#include <lib/utils.h>

void vga_init();
void vga_newline();
void vga_putchar(char ch);
void vga_setcolor(char color_code);
void vga_restore_color();
void vga_puts(char *str);
__attribute__((format(printf, 1, 2))) void vga_printf(const char *fmt, ...);