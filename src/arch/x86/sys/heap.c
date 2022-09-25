#include <sys/heap.h>
#include <lib/mutex.h>

typedef struct
{
    size_t heap_start;
    size_t heap_end;
    size_t next;
    uint32_t mutex_lock;
} TempAllocator;

static TempAllocator allocator = {
    .heap_start = 0x2000000,
    .heap_end = 0x2000000 + (100 * 1024), // 100Kib of heap space
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
    size_t alloc_start = align(allocator.next, 8);
    size_t alloc_end = alloc_start + size;

    if (alloc_end > allocator.heap_end)
    {
        return NULL;
    }

    spin_lock(&allocator.mutex_lock);
    allocator.next = alloc_end;
    spin_unlock(&allocator.mutex_lock);

    return (void *)alloc_start;
}

/* rollback to ptr */
void temp_rollback(void *ptr)
{
    spin_lock(&allocator.mutex_lock);
    allocator.next = (size_t)ptr;
    spin_unlock(&allocator.mutex_lock);
}