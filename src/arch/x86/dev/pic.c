#include <dev/pic.h>
#include <dev/serial.h>

#define WAIT_PORT 0x80

#define PIC1 0x20 /* IO base address for master PIC */
#define PIC2 0xA0 /* IO base address for slave PIC */
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2 + 1)


#define CMD_INIT 0x11
#define CMD_MODE_8086 0x01  
#define CMD_END_OF_INTERRUPT 0x20

/*
    "Allegedly" writing to 0x80 takes long enough to make everything work on most
    hardware.
*/
void wait()
{
    outb(WAIT_PORT, 0);
}

void pic_init()
{
    serial_init(WAIT_PORT);

    unsigned char a1, a2;

    a1 = inb(PIC1_DATA); // save masks
    a2 = inb(PIC2_DATA);

    // starts the initialization sequence
    // sending 3 bytes to initilize PIC
    outb(PIC1_COMMAND, CMD_INIT);
    wait();
    outb(PIC2_COMMAND, CMD_INIT);
    wait();

    // define PIC offset(s)
    outb(PIC1_DATA, PIC_1_OFFSET);
    wait();
    outb(PIC2_DATA, PIC_2_OFFSET);
    wait();

    outb(PIC1_DATA, 4);
    wait();
    outb(PIC2_DATA, 2);
    wait();

    // setup 8086 mode
    outb(PIC1_DATA, CMD_MODE_8086);
    wait();
    outb(PIC2_DATA, CMD_MODE_8086);
    wait();

    outb(PIC1_DATA, a1); // restore saved masks.
    outb(PIC2_DATA, a2);
}

// Notify the interrupt has been processed
void pic_eoi()
{
    outb(PIC1_COMMAND, CMD_END_OF_INTERRUPT);
    wait();
}

void pic_disable()
{
    // Disbale PIC
    outb(0xA1, 0xFF);
    outb(0x21, 0xFF);
}