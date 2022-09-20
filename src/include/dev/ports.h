#pragma once

#include <lib/types.h>

void outb(uint16_t port, uint8_t byte);
uint8_t inb(uint16_t port);