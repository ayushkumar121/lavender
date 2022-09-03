#include <sys/syscalls.h>

#include <dev/serial.h>
#include <lib/print.h>

volatile void syscall(int syscall_index)
{
    __asm__("mov eax, %0" ::"a"(syscall_index));
    __asm__("int 0x80");
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