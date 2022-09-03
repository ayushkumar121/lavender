#include <interrupts/isr.h>

#include <lib/print.h>
#include <sys/syscalls.h>

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

    __asm__("hlt");
}

__attribute__((interrupt)) void double_fault_handler(InterruptFrame32 *frame, uint32_t error_code)
{
    setcolor(VGA_COLOR_RED);
    printf("[EXCEPTION] Double fault\n");
    restore_color();
    __asm__("hlt");
}

__attribute__((interrupt)) void syscall_handler(InterruptFrame32 *frame)
{
    register int syscall_index __asm__("eax");
    switch (syscall_index)
    {
    case 1:
        syscall_test01();
        break;
    case 2:
        syscall_test02();
        break;

    default:
        break;
    }

    frame->ip++;
}