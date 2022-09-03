#pragma once

#include "../interrupts/isr.c"

#define MAX_SYSCALLS 2

#define SYSCALL_01 1
#define SYSCALL_02 2

void kernel_syscall(int syscall_index)
{
    asm("mov eax, edi");
    asm("int 0x80");
}

void syscall_test01(void)
{
    setcolor(VGA_COLOR_CYAN);
    printf("[SYSCALL] Syscall 1 called\n");
    restore_color();
}

void syscall_test02(void)
{
    setcolor(VGA_COLOR_CYAN);
    printf("[SYSCALL] Syscall 2 called\n");
    restore_color();
}

__attribute__((interrupt)) void syscall_hanlder(InterruptFrame32 *frame)
{
    register int i asm("eax");

    switch (i)
    {
    case 0:
        syscall_test01();
        break;
    case 1:
        syscall_test02();
        break;

    default:
        break;
    }

    frame->ip++;
}
