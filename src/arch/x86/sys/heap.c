#include <sys/heap.h>
#include <lib/mutex.h>

#include <gfx/vga.h>

typedef struct
{
    size_t heap_start;
    size_t heap_end;
    size_t next;
    uint32_t mutex_lock;
} BumpAllocator;

static BumpAllocator temp_allocator = {
    .heap_start = 0x2000000,
    .heap_end = 0x2000000 + (10 * 1024), // 10Kib of heap space
    .next = 0x2000000,
    .mutex_lock = MUTEX_UNLOCKED,
};

static size_t align(size_t addr, size_t alignment)
{
    size_t rem = addr % alignment;

    if (rem == 0)
    {
        return addr;
    }
    {
        return addr - rem + alignment;
    }
}

void *temp_alloc(size_t size)
{
    // Align to 8 bytes
    size_t alloc_start = align(temp_allocator.next, 8);
    size_t alloc_end = alloc_start + size;

    if (alloc_end > temp_allocator.heap_end)
    {
        return NULL;
    }

    spin_lock(&temp_allocator.mutex_lock);
    temp_allocator.next = alloc_end;
    spin_unlock(&temp_allocator.mutex_lock);

    return (void *)alloc_start;
}

void temp_rollback(void *ptr)
{
    spin_lock(&temp_allocator.mutex_lock);
    temp_allocator.next = (size_t)ptr;
    spin_unlock(&temp_allocator.mutex_lock);
}

typedef struct Node
{
    struct Node *next;
    size_t size;
} Node;

typedef struct
{
    Node head;
    uint32_t mutex_lock;
} LinkedListAllocator;

static LinkedListAllocator allocator = {
    .head = {
        .next = (Node *)0x3000000,
        .size = 0,
    },
    .mutex_lock = MUTEX_UNLOCKED,
};

void alloc_init()
{
    allocator.head.next->next = NULL;
    allocator.head.next->size = 100 * 1024; // 100Kib of heap space
}

void *alloc(size_t size)
{
    // Align memory size to sizeof Node
    size = align(size, sizeof(Node));

    // Find free region of "size"
    Node *prev = &(allocator.head);
    Node *cur = allocator.head.next;

    while (cur != NULL)
    {
        if (cur->size >= size)
            break;

        prev = cur;
        cur = cur->next;
    }

    if (cur == NULL)
        return NULL;

    // Now cur should have pointer to next available memory

    spin_lock(&allocator.mutex_lock);
    prev->next = cur + size;
    prev->next->next = cur->next;
    prev->next->size = cur->size - size;
    spin_unlock(&allocator.mutex_lock);

    return cur;
}

void free(void *ptr, size_t size)
{
    // Making a copy of current head
    Node temp = allocator.head;

    spin_lock(&allocator.mutex_lock);
    allocator.head.next = (Node *)ptr;
    allocator.head.next->size = size;
    allocator.head.next->next = temp.next;
    spin_unlock(&allocator.mutex_lock);
}