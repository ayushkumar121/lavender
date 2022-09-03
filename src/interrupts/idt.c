#pragma once

#include <utils.c>

#include "isr.c"
#include "syscall.c"

// flags
#define TRAP_GATE 0x8F     // 32 bit trap gate (p=1, dpl=0b00, type=0b1111 => type_attributes=1000_1111b=0x8F)
#define INT_GATE 0x8E      // 32 bit int gate  (p=1, dpl=0b00, type=0b1110 => type_attributes=0b1000_1110=0x8E)
#define INT_GATE_USER 0xEE // 32 bit int gate called from user code

typedef struct
{
    uint16_t offset_1;       // offset bits 0..15
    uint16_t selector;       // a code segment selector in GDT or LDT
    uint8_t zero;            // unused, set to 0
    uint8_t type_attributes; // gate type, dpl, and p fields
    uint16_t offset_2;       // offset bits 16..31
} __attribute__((packed)) IdtEntry32;

typedef struct
{
    uint16_t size;
    uint32_t offset;
} __attribute__((packed)) IdtDescriptor32;

IdtEntry32 idt_entries[256];
IdtDescriptor32 idt_descriptor;

IdtEntry32 idt_entry(void *handler_ptr, uint8_t flags)
{
    IdtEntry32 idt_entry = {0};
    idt_entry.offset_1 = (uint32_t)handler_ptr & 0xFFFF;
    idt_entry.offset_2 = (uint32_t)handler_ptr >> 16;
    idt_entry.type_attributes = flags;
    idt_entry.selector = 0x08;

    return idt_entry;
}

extern void idt_load();

void idt_init()
{
    uint16_t idt_size = sizeof idt_entries;

    // Setting up the descriptor table
    idt_descriptor.size = idt_size - 1;
    idt_descriptor.offset = (uint32_t)&idt_entries;

    memset(&idt_entries, 0, idt_size);

    // CPU exceptions
    idt_entries[0] = idt_entry(divide_by_zero_handler, TRAP_GATE);
    idt_entries[8] = idt_entry(double_fault_handler, TRAP_GATE);
    idt_entries[13] = idt_entry(page_fault_handler, TRAP_GATE);

    // Syscall handler
    idt_entries[128] = idt_entry(syscall_hanlder, INT_GATE_USER);

    // defined in kernel_entry.asm
    idt_load();
}