#pragma once

#include <lib/types.h>

#define NULL (void*)0

void *temp_alloc(size_t size);
void temp_rollback(void *ptr);