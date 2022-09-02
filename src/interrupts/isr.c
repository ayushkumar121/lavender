#pragma once

#include "../lib/utils.c"
#include "../lib/vga.c"

typedef struct InterruptFrame32
{
    uint16_t ip;
    uint16_t cs;
    uint16_t flags;
    uint16_t sp;
    uint16_t ss;
} __attribute__((packed)) InterruptFrame32;

 
__attribute__((interrupt)) 
void divide_by_zero_handler(InterruptFrame32* frame)
{
    vga_setcolor(VGA_COLOR_RED);
    vga_puts("[EXCEPTION] Divide by zero\n"); 
    vga_restore_color();

    frame->ip++;
}


__attribute__((interrupt)) 
void page_fault_handler(InterruptFrame32* frame, uint32_t error_code)
{
    vga_setcolor(VGA_COLOR_RED);
    vga_puts("[EXCEPTION] Page fault\n");
    vga_restore_color();

    asm("hlt");
}


__attribute__((interrupt)) 
void double_fault_handler(InterruptFrame32* frame, uint32_t error_code)
{
    vga_setcolor(VGA_COLOR_RED);
    vga_puts("[EXCEPTION] Double fault\n");
    vga_restore_color();
    asm("hlt");
}