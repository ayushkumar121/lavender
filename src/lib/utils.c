#pragma once

#include <stdarg.h>

typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long int uint64_t;

typedef unsigned long int size_t;

#define true 1
#define false 0

typedef struct 
{
    char data[128];
} ShortString;


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

void itoa(int value, char *buf)
{
    int k = 0;
    while (value)
    {
        int digit = value % 10;
        value = value / 10;

        buf[k++] = digit + '0';
    }

    strrev(buf);
}

void *memset(void *s, int c, size_t n)
{
    unsigned char *buf = (unsigned char *)s;
    for (size_t i; i < n; ++i)
    {
        buf[i] = c;
    }

    return s;
}
