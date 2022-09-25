#pragma once

#include <lib/types.h>

#define NULL (void*)0

/* Temp allocator */
/*
    This uses bump allocator to allocator to temporaly allocate memory
    and rollback rollbacks it to a previous state
*/
void *temp_alloc(size_t size);
void temp_rollback(void *ptr);

/* Linked List allocator */
/*
    This alloactor should be used for large allocations 
    that you free on arbitray order

    This creates a lot of heap fragmentation and lookup time is also high
    therefor use this carefully
*/

void alloc_init();
void *alloc(size_t size);
void free(void *ptr, size_t size);