#include <dev/serial.h>
#include <dev/ports.h>

#include <lib/utils.h>
#include <lib/sstring.h>

#include <stdarg.h>

inline static int serial_empty(uint16_t port)
{
    return inb(port + 5) & 0x20;
}

static void serial_puchar(uint16_t port, char a)
{
    while (serial_empty(port) == 0)
        ;

    outb(port, a);
}

static void serial_puts(uint16_t port, char *str)
{
    while (*str)
    {
        serial_puchar(port, *str++);
    }
}

int serial_init(const uint16_t port)
{
    outb(port + 1, 0x00); // Disable all interrupts
    outb(port + 3, 0x80); // Enable DLAB (set baud rate divisor)
    outb(port + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
    outb(port + 1, 0x00); //                  (hi byte)
    outb(port + 3, 0x03); // 8 bits, no parity, one stop bit
    outb(port + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
    outb(port + 4, 0x0B); // IRQs enabled, RTS/DSR set

    outb(port + 4, 0x1E); // Set in loopback mode, test the serial chip
    outb(port + 0, 0xAE); // Test serial chip (send byte 0xAE and check if serial returns same byte)

    // Check if serial is faulty (i.e: not same byte as sent)
    if (inb(port + 0) != 0xAE)
    {
        return SERIAL_FAILED;
    }

    // If serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    outb(port + 4, 0x0F);
    return SERIAL_OK;
}

// TODO: add mutex here as well
__attribute__((format(printf, 2, 3))) void serial_printf(const uint16_t port, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    SString ss = ss_vprintf(fmt, args);
    serial_puts(port, ss.data);

    va_end(args);
}