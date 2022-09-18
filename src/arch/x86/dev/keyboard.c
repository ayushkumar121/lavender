#include <dev/keyboard.h>

#include <dev/interrupts.h>
#include <dev/pic.h>
#include <dev/serial.h>

#include <gfx/vga.h>

#define KEYBOARD_INT_INDEX (PIC_1_OFFSET + 1)

static char keymap[0xff];

// TODO: add mutex here
static char key = 0;
static char key_event = 0;

void keymap_init()
{
    keymap[0x1e] = 'a';
    keymap[0x1f] = 's';
    keymap[0x20] = 'd';
    keymap[0x21] = 'f';
    keymap[0x22] = 'g';
    keymap[0x23] = 'h';
    keymap[0x24] = 'j';
    keymap[0x25] = 'k';
    keymap[0x26] = 'l';
    keymap[0x10] = 'q';
    keymap[0x11] = 'w';
    keymap[0x12] = 'e';
    keymap[0x13] = 'r';
    keymap[0x14] = 't';
    keymap[0x15] = 'y';
    keymap[0x16] = 'u';
    keymap[0x17] = 'i';
    keymap[0x18] = 'o';
    keymap[0x19] = 'p';
    keymap[0x2c] = 'z';
    keymap[0x2d] = 'x';
    keymap[0x2e] = 'c';
    keymap[0x2f] = 'v';
    keymap[0x30] = 'b';
    keymap[0x31] = 'n';
    keymap[0x32] = 'm';
    keymap[0x39] = ' ';
    keymap[0x04] = '3';
    keymap[0x08] = '7';
    keymap[0x02] = '1';
    keymap[0x06] = '5';
    keymap[0x0A] = '9';
    keymap[0x03] = '2';
    keymap[0x0B] = '0';
    keymap[0x07] = '6';
    keymap[0x05] = '4';
    keymap[0x09] = '8';

    keymap[0x02] = '1';
    keymap[0x03] = '2';
    keymap[0x04] = '3';
    keymap[0x05] = '4';
    keymap[0x06] = '5';
    keymap[0x07] = '6';
    keymap[0x08] = '7';
    keymap[0x09] = '8';
    keymap[0x0a] = '9';
    keymap[0x0b] = '0';

    keymap[0x1c] = '\n';
}

__attribute__((interrupt)) void keyboard_handler(InterruptFrame *frame)
{
    char scancode = inb(0x60);

    key = scancode & ~(1 << 7);
    key_event = scancode & (1 << 7);

    pic_eoi(KEYBOARD_INT_INDEX);
}

char keyboard_getch(int event)
{
    int result = 0;
    while (key == 0 || key_event != event)
        ;

    result = key;
    key = 0;

    return keymap[result];
}

void keyboard_init()
{
    keymap_init();
    interrupts_add_handler(KEYBOARD_INT_INDEX, keyboard_handler, INT_GATE);
}