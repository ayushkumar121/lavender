#pragma once

#include <utils.c>
#include <print.c>

typedef struct
{
    uint16_t ip;
    uint16_t cs;
    uint16_t flags;
    uint16_t sp;
    uint16_t ss;
} __attribute__((packed)) InterruptFrame32;

__attribute__((interrupt)) void divide_by_zero_handler(InterruptFrame32 *frame)
{
    setcolor(VGA_COLOR_RED);
    printf("[EXCEPTION] Divide by zero\n");
    restore_color();

    frame->ip++;
}

__attribute__((interrupt)) void page_fault_handler(InterruptFrame32 *frame, uint32_t error_code)
{
    setcolor(VGA_COLOR_RED);
    printf("[EXCEPTION] Page fault\n");
    restore_color();

    asm("hlt");
}

__attribute__((interrupt)) void double_fault_handler(InterruptFrame32 *frame, uint32_t error_code)
{
    setcolor(VGA_COLOR_RED);
    printf("[EXCEPTION] Double fault\n");
    restore_color();
    asm("hlt");
}