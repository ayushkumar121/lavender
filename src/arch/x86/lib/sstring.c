#include <lib/sstring.h>
#include <lib/utils.h>

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

SString ss_vprintf(const char *fmt, va_list args)
{
    SString result = {0};
    uint8_t k = 0;

    while (*fmt)
    {
        char ch = *fmt++;
        if (ch == '%' && k < MAX_SSTRING - 1)
        {
            char tp = *(fmt++);
            switch (tp)
            {
            case 'b':
            {
                SString ss = {0};
                int32_t num = va_arg(args, int32_t);
                itoa(num, ss.data, 2);

                result = ss_cat(result, ss_from_cstr("0b"));
                k += 2;

                result = ss_cat(result, ss);
                k += ss_len(ss);
            }
            break;

            case 'd':
            {
                SString ss = {0};
                int32_t num = va_arg(args, int32_t);
                itoa(num, ss.data, 10);

                result = ss_cat(result, ss);
                k += ss_len(ss);
            }
            break;

            case 'l':
            {
                SString ss = {0};
                int64_t num = va_arg(args, int64_t);
                itoa(num, ss.data, 10);

                result = ss_cat(result, ss);
                k += ss_len(ss);
            }
            break;

            case 'x':
            {
                SString ss = {0};
                uint64_t num = va_arg(args, uint64_t);
                itoa(num, ss.data, 16);

                result = ss_cat(result, ss_from_cstr("0x"));
                k += 2;

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
                k += ss_len(ss);
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

    return result;
}

__attribute__((format(printf, 1, 2))) SString ss_printf(const char *fmt, ...)
{
    SString result = {0};

    va_list args;
    va_start(args, fmt);

    result = ss_vprintf(fmt, args);

    va_end(args);
    return result;
}