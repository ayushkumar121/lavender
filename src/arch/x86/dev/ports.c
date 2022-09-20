#include <dev/ports.h>

void outb(uint16_t port, uint8_t byte)
{
 __asm__("out %0, %1"
            :
            : "Nd"(port), "a"(byte)
            :);
}

uint8_t inb(uint16_t port)
{
   uint8_t byte;
    __asm__("in %0, %1"
            : "=a"(byte)
            : "Nd"(port)
            :);
    return byte;
}