#include <sys/syscalls.h>

#include <gfx/vga.h>
#include <gfx/vga_colors.h>

typedef size_t (*SyscallHandler)(size_t);

static size_t syscall_putchar(size_t data)
{
    vga_putchar(data);
    return 0;
}

static size_t syscall_getkey(size_t data)
{
    return 0;
}

static SyscallHandler syscalls[SYSCALL_COUNT] = 
{
   [SYSCALL_PUTCHAR] = syscall_putchar,
   [SYSCALL_GETKEY] = syscall_getkey,
};

size_t syscall(size_t syscall_index, size_t argument)
{
    if (syscall_index < SYSCALL_COUNT)
    {
        return syscalls[syscall_index](argument);
    }
    return 0;
}
