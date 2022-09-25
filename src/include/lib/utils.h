#pragma once

#include <lib/types.h>

#define ARRSIZE(arr) (sizeof(arr) / sizeof(arr[0]))

void strrev(char *str);
size_t strlen(const char *str);
void itoa(int64_t value, char *buf, int base);
void *memset(void *s, int c, size_t n);
void *memcpy(void *d, void *s, size_t n);