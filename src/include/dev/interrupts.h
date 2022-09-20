#pragma once

#include <lib/types.h>

#define TRAP_GATE                       0x8F // 32 bit trap gate (p=1, dpl=0b00, type=0b1111 => type_attributes=1000_1111b=0x8F)
#define INT_GATE                        0x8E // 32 bit int gate  (p=1, dpl=0b00, type=0b1110 => type_attributes=0b1000_1110=0x8E)
#define INT_GATE_USER                   0xEE // 32 bit int gate called from user code

#define IRQ_DIVIDE_BY_ZERO              0x0
#define IRQ_NON_MASKABLE_INT            0x2
#define IRQ_DOUBLE_FUALT                0x8
#define IRQ_INVALID_TSS                 0xC
#define IRQ_GENERAL_PROTECTION_FAULT    0xD
#define IRQ_PAGE_FAULT                  0xE
#define IRQ_SYSCALL                     0x80

typedef struct __attribute__((packed))
{
    uint64_t ip;
    uint64_t cs;
    uint64_t flags;
    uint64_t sp;
    uint64_t ss;
} InterruptFrame;

void interrupts_init();
void interrupts_load();
void interrupts_add_handler(uint8_t handler_index, void *handler, uint8_t flags);