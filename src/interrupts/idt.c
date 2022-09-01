#pragma once

#include "../lib/vga.c"
#include "../lib/utils.c"


#define TRAP_GATE 0x8F   
#define INT_GATE 0x8E 
#define INT_GATE_USER 0xEE 

typedef struct IdtEntry32 
{
  uint16_t offset_1;       // offset bits 0..15
  uint16_t selector;       // a code segment selector in GDT or LDT
  uint8_t zero;            // unused, set to 0
  uint8_t type_attributes; // gate type, dpl, and p fields
  uint16_t offset_2;       // offset bits 16..31
} __attribute__((packed)) IdtEntry32;


typedef struct IdtDescriptor32
{
    uint16_t size;
    uint32_t offset;
} __attribute__((packed)) IdtDescriptor32;

typedef struct InterruptFrame32
{
    uint16_t ip;
    uint16_t cs;
    uint16_t flags;
    uint16_t sp;
    uint16_t ss;
} __attribute__((packed)) InterruptFrame32;
 
__attribute__((interrupt)) void divide_by_zero_handler(InterruptFrame32* frame)
{
    vga_puts("[ERROR] Caught Divide by zero\n", VGA_COLOR_RED); 
    frame->ip++;
}


__attribute__((interrupt)) void page_fault_handler(InterruptFrame32* frame, uint32_t error_code)
{
    vga_puts("[ERROR] Page fault\n", VGA_COLOR_RED);
    asm("hlt");
}


__attribute__((interrupt)) void double_fault_handler(InterruptFrame32* frame, uint32_t error_code)
{
    vga_puts("[ERROR] Double fault\n", VGA_COLOR_RED);
    asm("hlt");
}

IdtEntry32 idt_entries[256];
IdtDescriptor32 idt_descriptor;


IdtEntry32 idt_entry(void *handler_ptr, uint8_t gate_type)
{
    IdtEntry32 idt_entry = {0};
    idt_entry.offset_1 = (uint32_t)handler_ptr & 0xFFFF;
    idt_entry.offset_2 =  (uint32_t)handler_ptr >> 16;
    idt_entry.type_attributes = gate_type;
    idt_entry.selector = 0x08;

    return idt_entry;
}

extern void idt_load();

void idt_init() 
{
    uint16_t idt_size = sizeof  idt_entries;
    // Setting up the descriptor table
    idt_descriptor.size = idt_size;
    idt_descriptor.offset = (uint32_t) &idt_entries;

    memset(&idt_entries, 0, idt_size);

    idt_entries[0] = idt_entry(divide_by_zero_handler, TRAP_GATE);    
    idt_entries[8] = idt_entry(double_fault_handler,  TRAP_GATE);    
    idt_entries[13] = idt_entry(page_fault_handler,  TRAP_GATE);    

    // defined in kernel_entry.asm
    idt_load();
}