#include <lib/sstring.h>
#include <lib/utils.h>

#include <stdarg.h>

size_t ss_len(SString a)
{
    size_t k = 0;
    char *ptr = a.data;
    while (*ptr++)
        k++;
    return k;
}

SString ss_from_cstr(const char *a)
{
    uint8_t k = 0;
    SString result = {0};

    while (*a)
    {
        result.data[k++] = *a++;
    }

    return result;
}

SString ss_cat(SString a, SString b)
{
    SString result = {0};
    size_t n = ss_len(a);
    size_t m = ss_len(b);

    uint8_t k = 0;
    for (size_t i = 0; i < n; ++i)
    {
        result.data[k++] = a.data[i];
    }

    for (size_t i = 0; i < m; ++i)
    {
        result.data[k++] = b.data[i];
    }

    return result;
}

__attribute__((format(printf, 1, 2))) SString ss_printf(const char *fmt, ...)
{
    SString result = {0};
    uint8_t k = 0;

    va_list args;
    va_start(args, fmt);

    while (*fmt)
    {
        char ch = *fmt++;
        if (ch == '%')
        {
            char tp = *(fmt++);
            switch (tp)
            {
            case 'd':
            {
                SString ss = {0};
                uint32_t num = va_arg(args, int);
                itoa(num, ss.data);

                result = ss_cat(result, ss);
                k += ss_len(ss);
            }
            break;

            case 'c':
            {
                char c = va_arg(args, int);
                result.data[k++] = c;
            }
            break;

            case 's':
            {
                char *s = va_arg(args, char *);
                SString ss = ss_from_cstr(s);
                result = ss_cat(result, ss);
            }
            break;

            default:
                break;
            }
        }
        else
        {
            result.data[k++] = ch;
        }
    }

    va_end(args);
    return result;
}