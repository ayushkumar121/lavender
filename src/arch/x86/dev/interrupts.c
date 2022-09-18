#include <dev/interrupts.h>

#include <gfx/vga.h>
#include <gfx/vga_colors.h>

#include <dev/serial.h>

#include <sys/syscalls.h>

typedef struct __attribute__((packed))
{
    uint16_t offset_1;       // offset bits 0..15
    uint16_t selector;       // a code segment selector in GDT or LDT
    uint8_t ist;             // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
    uint8_t type_attributes; // gate type, dpl, and p fields
    uint16_t offset_2;       // offset bits 16..31
    uint32_t offset_3;       // offset bits 32..63
    uint32_t zero;           // reserved
} IdtEntry;

typedef struct __attribute__((packed))
{
    uint16_t size;
    uint64_t offset;
} IdtDescriptor;

static IdtDescriptor idt_descriptor;

// defined in BSS section
extern IdtEntry idt_entries[256];

void interrupts_init()
{
    uint16_t idt_size = sizeof(IdtEntry) * 256;

    // Setting up the descriptor table
    idt_descriptor.size = idt_size - 1;
    idt_descriptor.offset = (uint64_t)&idt_entries;

    for (size_t i = 0; i < 256; ++i)
    {
        if (i == 8 || (i > 9 && i < 15) || i == 17 || i == 21 || i == 29 || i == 30)
        {
            interrupts_add_handler(i, default_interrupt_handler_errcode, TRAP_GATE);
        }
        else
        {
            interrupts_add_handler(i, default_interrupt_handler, TRAP_GATE);
        }
    }

    interrupts_add_handler(IRQ_DIVIDE_BY_ZERO, divide_by_zero_handler, TRAP_GATE);
    interrupts_add_handler(IRQ_PAGE_FAULT, page_fault_handler, TRAP_GATE);
    interrupts_add_handler(IRQ_SYSCALL, syscall_handler, INT_GATE_USER);
}

void interrupts_load()
{
    __asm__ volatile("lidt %0" ::"m"(idt_descriptor));
    __asm__ volatile("sti");
}

void interrupts_add_handler(uint8_t handler_index, void *handler, uint8_t flags)
{
    uint64_t handler_ptr = (uint64_t)handler;

    idt_entries[handler_index].offset_1 = handler_ptr & 0xFFFF;
    idt_entries[handler_index].offset_2 = handler_ptr >> 16 & 0xFFFF;
    idt_entries[handler_index].offset_3 = handler_ptr >> 34 & 0xFFFFFFFF;
    idt_entries[handler_index].type_attributes = flags;
    idt_entries[handler_index].ist = 0;
    idt_entries[handler_index].selector = 0x08;
}

// Exceptions
__attribute__((interrupt)) void default_interrupt_handler(InterruptFrame *frame)
{
    vga_setcolor(VGA_COLOR_RED);
    vga_printf("[Exception] Exception occured\n");
    vga_restore_color();
    __asm__("hlt");
}

__attribute__((interrupt)) void default_interrupt_handler_errcode(InterruptFrame *frame, uint64_t error_code)
{
    vga_setcolor(VGA_COLOR_RED);
    vga_printf("[Exception] Exception occured :(%d)\n", (int16_t)error_code);
    vga_restore_color();
    __asm__("hlt");
}

__attribute__((interrupt)) void divide_by_zero_handler(InterruptFrame *frame)
{
    vga_setcolor(VGA_COLOR_RED);
    vga_printf("[Exception] Divide by zero\n");
    vga_restore_color();

    frame->ip++;
}

__attribute__((interrupt)) void page_fault_handler(InterruptFrame *frame, uint64_t error_code)
{
    vga_setcolor(VGA_COLOR_RED);
    vga_printf("[Exception] Page fault\n");
    vga_restore_color();

    __asm__("hlt");
}

// Syscalls
__attribute__((interrupt)) void syscall_handler(InterruptFrame *frame)
{
    register int syscall_index __asm__("eax");
    handle_syscall(syscall_index);
    frame->ip++;
}