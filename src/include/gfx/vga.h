#pragma once

#include <lib/utils.h>

void vga_setcolor(char color_code);
void vga_restore_color();

void vga_printf(const char *fmt, ...);