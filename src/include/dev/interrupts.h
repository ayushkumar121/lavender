#pragma once

#include <lib/types.h>

#define TRAP_GATE 0x8F     // 32 bit trap gate (p=1, dpl=0b00, type=0b1111 => type_attributes=1000_1111b=0x8F)
#define INT_GATE 0x8E      // 32 bit int gate  (p=1, dpl=0b00, type=0b1110 => type_attributes=0b1000_1110=0x8E)
#define INT_GATE_USER 0xEE // 32 bit int gate called from user code

// typedef struct {
//     uint64_t ip;
//     uint64_t cs;
//     uint64_t flags;
//     uint64_t sp;
//     uint64_t ss;
// } InterruptFrame;

// typedef  __attribute__((interrupt)) void (*InterruptHandler) (InterruptFrame*);

void interrupts_init();
void interrupts_add_handler(uint8_t handler_index, void *handler_ptr, uint8_t flags);