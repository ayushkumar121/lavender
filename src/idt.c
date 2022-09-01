#ifndef IDT_H
#define IDT_H

#include "types.h"
#include "vga.c"
#include "utils.c"

void divide_by_zero_handler()
{
    vga_puts("Caught Divide by zero\n", VGA_COLOR_RED); 
}


void page_fault_handler()
{
    vga_puts("Page fault", VGA_COLOR_RED);
    asm("hlt");
}


void double_fault_handler()
{
    vga_puts("Double fault", VGA_COLOR_RED);
    asm("hlt");
}

typedef struct IdtEntry32 
{
  uint16_t offset_1;       // offset bits 0..15
  uint16_t selector;       // a code segment selector in GDT or LDT
  uint8_t zero;            // unused, set to 0
  uint8_t type_attributes; // gate type, dpl, and p fields
  uint16_t offset_2;       // offset bits 16..31
} __attribute__((packed)) IdtEntry32;


enum IdtEntryType32
{
    INTERRUPT_GATE = 0x8E,  // Can be disabled
    TRAP_GATE = 0x8F        // Cannot be disabled used for exceptions
};

typedef struct IdtDescriptor32
{
    uint16_t size;
    uint32_t offset;
} __attribute__((packed)) IdtDescriptor32;

IdtEntry32 idt_entries[256];
IdtDescriptor32 idt_descriptor;

extern uint16_t gdt_selector;

IdtEntry32 idt_entry(uint32_t handler_ptr, char gate_type)
{
    IdtEntry32 idt_entry = {0};
    idt_entry.offset_1 = handler_ptr & 0xFFFF;
    idt_entry.offset_2 = handler_ptr >> 16;
    idt_entry.type_attributes = gate_type;
    idt_entry.selector = gdt_selector;

    return idt_entry;
}

// defined in kernel_entry.asm
extern void idt_load();
extern void isr_divide_zero();

void idt_init() 
{
    uint16_t idt_size= sizeof(struct IdtEntry32) * 256;

    // Setting up the descriptor table
    idt_descriptor.size =idt_size - 1;
    idt_descriptor.offset = (uint32_t) &idt_entries;

    memset(&idt_entries, 0, idt_size);

    idt_entries[0] = idt_entry((uint32_t)isr_divide_zero, (uint8_t) TRAP_GATE);    
    idt_entries[8] = idt_entry((uint32_t)double_fault_handler, (uint8_t) TRAP_GATE);    
    idt_entries[13] = idt_entry((uint32_t)page_fault_handler, (uint8_t) TRAP_GATE);    

    idt_load();
}


#endif