#include <dev/interrupts.h>

struct interrupt_frame
{
    uint64_t ip;
    uint64_t cs;
    uint64_t flags;
    uint64_t sp;
    uint64_t ss;
};

__attribute__((interrupt)) void interrupt_handler(struct interrupt_frame *frame)
{
    __asm__ ("hlt");
}

void interrupts_init()
{
}

void interrupts_add_handler(uint8_t handler_index, void *handler_ptr, uint8_t flags)
{
}