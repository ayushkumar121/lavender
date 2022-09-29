#include <dev/keyboard.h>

#include <dev/interrupts.h>
#include <dev/pic.h>
#include <dev/ports.h>
#include <dev/serial.h>

#include <gfx/vga.h>
#include <lib/mutex.h>

typedef struct
{
    char key;
    char key_event;

    uint32_t mutex_lock;
} Keyboard;

static const char keymap[0xff] = {
    [0x1e] = 'a',
    [0x1f] = 's',
    [0x20] = 'd',
    [0x21] = 'f',
    [0x22] = 'g',
    [0x23] = 'h',
    [0x24] = 'j',
    [0x25] = 'k',
    [0x26] = 'l',
    [0x10] = 'q',
    [0x11] = 'w',
    [0x12] = 'e',
    [0x13] = 'r',
    [0x14] = 't',
    [0x15] = 'y',
    [0x16] = 'u',
    [0x17] = 'i',
    [0x18] = 'o',
    [0x19] = 'p',
    [0x2c] = 'z',
    [0x2d] = 'x',
    [0x2e] = 'c',
    [0x2f] = 'v',
    [0x30] = 'b',
    [0x31] = 'n',
    [0x32] = 'm',
    [0x39] = ' ',
    [0x33] = ',',
    [0x37] = '*',
    [0x02] = '1',
    [0x03] = '2',
    [0x04] = '3',
    [0x05] = '4',
    [0x06] = '5',
    [0x07] = '6',
    [0x08] = '7',
    [0x09] = '8',
    [0x0A] = '9',
    [0x0B] = '0',
    [0x1C] = '\n',
    [0x0E] = '\b',
};

static Keyboard keyboard = {
    .key = 0,
    .key_event = 0,
    .mutex_lock = MUTEX_UNLOCKED,
};

__attribute__((interrupt)) static void keyboard_handler(InterruptFrame *frame)
{
    char scancode = inb(0x60);

    spin_lock(&keyboard.mutex_lock);
    keyboard.key = scancode & ~(1 << 7);
    keyboard.key_event = scancode & (1 << 7);
    spin_unlock(&keyboard.mutex_lock);

    pic_eoi(KEYBOARD_INT_INDEX);
}

char keyboard_getch(int event)
{
    int result = 0;

    while (keyboard.key == 0 || keyboard.key_event != event)
        ;

    spin_lock(&keyboard.mutex_lock);
    result = keyboard.key;
    keyboard.key = 0;
    spin_unlock(&keyboard.mutex_lock);

    return keymap[result];
}

void keyboard_init()
{
    interrupts_add_handler(KEYBOARD_INT_INDEX, keyboard_handler, INT_GATE);
}