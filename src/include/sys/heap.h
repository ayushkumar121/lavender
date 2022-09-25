#pragma once

#include <lib/types.h>

#define NULL (void*)0

void *heap_alloc(size_t size);
void heap_free(void *ptr);