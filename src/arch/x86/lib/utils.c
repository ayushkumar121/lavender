
#include <lib/utils.h>

size_t strlen(const char *str)
{
    size_t size = 0;
    while (*str++)
        size++;
    return size;
}

void strrev(char *str)
{
    int n = strlen(str);
    for (int i = 0; i < n / 2; i++)
    {
        char temp = str[i];
        str[i] = str[n - i - 1];
        str[n - i - 1] = temp;
    }
}

void itoa(size_t value, char *buf)
{
    bool neg = false;
    size_t k = 0;

    if (value == 0)
    {
        buf[k++] = '0';
        return;
    }

    if (value < 0)
    {
        value *= -1;
        neg = true;
    }

    while (value)
    {
        int rem = value % 10;
        value = value / 10;

        buf[k++] = rem + '0';
    }

    if (neg)
    {
        buf[k++] = '-';
    }

    strrev(buf);
}

void *memset(void *s, int c, size_t n)
{
    int *buf = (int *)s;
    for (size_t i = 0; i < n; ++i)
    {
        buf[i] = c;
    }

    return s;
}
