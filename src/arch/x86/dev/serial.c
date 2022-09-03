#include <dev/serial.h>

static void outb(uint16_t port, uint8_t byte)
{
    __asm__("out %0, %1"
        :
        : "Nd"(port), "a"(byte)
        :);
}

static uint8_t inb(uint16_t port)
{
    uint8_t byte;
    __asm__("in %0, %1"
        : "=a"(byte)
        : "Nd"(port)
        :);
    return byte;
}

void serial_init(const uint8_t port)
{
}

void serial_printf(const uint8_t port, const char *fmt, ...)
{
}




// void outb(uint16_t port, uint8_t byte)
// {
//     asm("out %0, %1"
//         :
//         : "Nd"(port), "a"(byte)
//         :);
// }

// uint8_t inb(uint16_t port)
// {
//     uint8_t byte;
//     asm("in %0, %1"
//         : "=a"(byte)
//         : "Nd"(port)
//         :);
//     return byte;
// }

// int init_serial()
// {
//     printf("[OK]");

//     outb(PORT + 1, 0x00); // Disable all interrupts
//     outb(PORT + 3, 0x80); // Enable DLAB (set baud rate divisor)
//     outb(PORT + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
//     outb(PORT + 1, 0x00); //                  (hi byte)
//     outb(PORT + 3, 0x03); // 8 bits, no parity, one stop bit
//     outb(PORT + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
//     outb(PORT + 4, 0x0B); // IRQs enabled, RTS/DSR set
//     outb(PORT + 4, 0x1E); // Set in loopback mode, test the serial chip
//     outb(PORT + 0, 0xAE); // Test serial chip (send byte 0xAE and check if serial returns same byte)


//     // Check if serial is faulty (i.e: not same byte as sent)
//     if (inb(PORT + 0) != 0xAE)
//     {
//         return 1;
//     }

//     // If serial is not faulty set it in normal operation mode
//     // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
//     outb(PORT + 4, 0x0F);
//     return 0;
// }

// int is_transmit_empty()
// {
//     return inb(PORT + 5) & 0x20;
// }

// void write_serial(char a)
// {
//     while (is_transmit_empty() == 0)
//         ;

//     outb(PORT, a);
// }
