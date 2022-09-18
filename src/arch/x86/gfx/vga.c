#include <gfx/vga.h>
#include <gfx/vga_colors.h>
#include <dev/serial.h>
#include <lib/sstring.h>

#include <stdarg.h>

#define VGA_ROWS 25
#define VGA_COLS 80

typedef struct
{
    char ascii_character;
    char color_code;
} __attribute__((packed)) VgaScreenChar;

typedef struct
{
    VgaScreenChar chars[VGA_ROWS][VGA_COLS];
} VgaBuffer;

typedef struct
{
    VgaBuffer *buffer;

    int row;
    int col;
    char color;
    char previous_color;
} VgaWriter;

// TODO: add a mutex to writer
static VgaWriter writer;

static void update_cursor()
{
    uint16_t pos = (VGA_ROWS - 1) * 80 + writer.col;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

static void vga_clearrow(int row)
{
    for (int col = 0; col < VGA_COLS; ++col)
    {
        writer.buffer->chars[row][col].ascii_character = ' ';
    }
}

static void vga_newline()
{

    for (int row = 1; row < VGA_ROWS; ++row)
    {
        for (int col = 0; col < VGA_COLS; ++col)
        {
            writer.buffer->chars[row - 1][col] = writer.buffer->chars[row][col];
        }
    }

    vga_clearrow(VGA_ROWS - 1);
    writer.col = 0;
}

static void vga_putchar(char ch)
{
    if (ch != '\n')
    {
        writer.buffer->chars[writer.row][writer.col].ascii_character = ch;
        writer.buffer->chars[writer.row][writer.col].color_code = writer.color;

        if (writer.col < VGA_COLS)
        {
            writer.col++;
        }
        else
        {
            vga_newline();
        }
    }
    else
    {
        vga_newline();
    }

    update_cursor();
}

static void vga_puts(char *str)
{
    while (*str)
    {
        vga_putchar(*str++);
    }
}

void vga_init()
{
    writer.buffer = (VgaBuffer *)0xb8000;
    writer.col = 0;
    writer.row = VGA_ROWS - 1;
    writer.color = VGA_COLOR_WHITE;
    writer.previous_color = VGA_COLOR_WHITE;
}

void vga_setcolor(char color_code)
{
    writer.previous_color = writer.color;
    writer.color = color_code;
}

void vga_restore_color()
{
    char temp = writer.color;
    writer.color = writer.previous_color;
    writer.previous_color = temp;
}

__attribute__((format(printf, 1, 2))) void vga_printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    SString ss = ss_vprintf(fmt, args);
    vga_puts(ss.data);

    va_end(args);
}