#pragma once

#include <lib/types.h>

#define MUTEX_LOCKED 1
#define MUTEX_UNLOCKED 0

void spin_lock(uint32_t *state);
void spin_unlock(uint32_t *state);