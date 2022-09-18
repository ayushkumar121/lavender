#pragma once

#include <lib/utils.h>

void vga_init();

void vga_setcolor(char color_code);
void vga_restore_color();

__attribute__((format(printf, 1, 2))) void vga_printf(const char *fmt, ...);