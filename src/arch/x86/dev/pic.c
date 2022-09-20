#include <dev/pic.h>

#include <dev/ports.h>
#include <dev/interrupts.h>

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

#define TIMER_INT_INDEX PIC_1_OFFSET

__attribute__((interrupt)) static void timer_handler(InterruptFrame *frame)
{
    // serial_printf(COM1, ".");
    pic_eoi(TIMER_INT_INDEX);
}

// "Allegedly" writing to 0x80 takes long enough to make everything work on most hardware.
void wait()
{
    outb(WAIT_PORT, 0);
}

void pic_init()
{
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

    interrupts_add_handler(TIMER_INT_INDEX, timer_handler, INT_GATE);
}

// Notify the interrupt has been processed
void pic_eoi(int interrupt_index)
{
    if (interrupt_index >= PIC_2_OFFSET)
    {
        outb(PIC2_COMMAND, CMD_END_OF_INTERRUPT);
    }
    outb(PIC1_COMMAND, CMD_END_OF_INTERRUPT);
}

void pic_disable()
{
    // Disbale PIC
    outb(0xA1, 0xFF);
    outb(0x21, 0xFF);
}