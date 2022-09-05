#pragma once

#include <gfx/vga_colors.h>

void setcolor(char color);
void restore_color();
__attribute__((format(printf, 1, 2))) void printf(const char *fmt, ...);