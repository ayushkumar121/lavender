#include <sys/syscalls.h>

#include <gfx/vga.h>
#include <gfx/vga_colors.h>

volatile void syscall(int syscall_index)
{
    __asm__("mov eax, %0" ::"a"(syscall_index));
    __asm__("int 0x80");
}

void handle_syscall(int syscall_index)
{
    static Syscall syscalls[] = {
        syscall_test01,
        syscall_test02,
    };

    if (syscall_index < MAX_SYSCALLS)
    {
        syscalls[syscall_index]();
    }
}

void syscall_test01(void)
{
    vga_setcolor(VGA_COLOR_CYAN);
    vga_printf("[SYSCALL] Syscall 1 called\n");
    vga_restore_color();
}

void syscall_test02(void)
{
    vga_setcolor(VGA_COLOR_CYAN);
    vga_printf("[SYSCALL] Syscall 2 called\n");
    vga_restore_color();
}