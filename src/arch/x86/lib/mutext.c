#include <lib/mutex.h>

void spin_lock(uint32_t *state)
{
    // Check if lock is free
    __asm__ volatile(".spin_wait:");
    __asm__ volatile("test %0, 1" ::"m"(*state));
    __asm__ volatile("jnz .spin_wait");

    // If lock is free try to aquire it
    __asm__ volatile("aquire_lock:");
    __asm__ volatile("lock bts %0, 0" ::"m"(*state));
    __asm__ volatile("jc .spin_wait");
}

void spin_unlock(uint32_t *state)
{
    // unsets the 0th bit and store the previous value in CF
    __asm__ volatile("lock btr %0, 0" ::"m"(*state));
}