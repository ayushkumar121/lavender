#pragma once

#include <lib/utils.h>
#include <lib/print.h>

#include <sys/syscalls.h>

typedef struct
{
    uint16_t ip;
    uint16_t cs;
    uint16_t flags;
    uint16_t sp;
    uint16_t ss;
} __attribute__((packed)) InterruptFrame32;

__attribute__((interrupt)) void divide_by_zero_handler(InterruptFrame32 *frame);
__attribute__((interrupt)) void page_fault_handler(InterruptFrame32 *frame, uint32_t error_code);
__attribute__((interrupt)) void double_fault_handler(InterruptFrame32 *frame, uint32_t error_code);
__attribute__((interrupt)) void syscall_handler(InterruptFrame32 *frame);