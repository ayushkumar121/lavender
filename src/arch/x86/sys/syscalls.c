#include <sys/syscalls.h>
#include <dev/serial.h>

volatile void syscall(int syscall_index)
{
    __asm__("mov eax, %0" ::"a"(syscall_index));
    __asm__("int 0x80");
}

void syscall_test01(void)
{
    serial_printf(COM1, "[SYSCALL] Syscall 1 called\n");
}

void syscall_test02(void)
{
    serial_printf(COM1, "[SYSCALL] Syscall 2 called\n");
}