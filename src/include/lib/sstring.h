#pragma once

#include <lib/types.h>

typedef struct 
{
    uint8_t length;
    char data[256];
} SString;


SString ss_from_cstr(const char *a);
SString ss_cat(SString a, SString b);

__attribute__((format(printf, 1, 2))) SString ss_printf(const char *fmt, ...);