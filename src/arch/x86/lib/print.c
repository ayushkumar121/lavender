#include <lib/print.h>
#include <lib/types.h>
#include <lib/utils.h>
#include <lib/string.h>

#include <gfx/vga.h>

#include <stdarg.h>

void setcolor(char color)
{
    vga_setcolor(color);
}

void restore_color()
{
    vga_restore_color();
}

__attribute__((format(printf, 1, 2))) void printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    while (*fmt)
    {
        char ch = *fmt++;
        if (ch == '%')
        {
            char tp = *fmt++;
            switch (tp)
            {
            case 'd':
            {
                ShortString ss;
                int num = va_arg(args, int);

                itoa(num, ss.data);
                vga_puts(ss.data);
            }
            break;

            case 'l':
            {
                ShortString ss;
                long num = va_arg(args, long);

                itoa(num, ss.data);
                vga_puts(ss.data);
            }
            break;

            case 'c':
            {
                char c = va_arg(args, int);
                vga_putchar(c);
            }
            break;

            case 's':
            {
                char *s = va_arg(args, char *);
                vga_puts(s);
            }
            break;

            default:
                break;
            }
        }
        else
        {
            vga_putchar(ch);
        }
    }

    va_end(args);
}