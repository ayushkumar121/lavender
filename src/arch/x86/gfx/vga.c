#include <gfx/vga.h>
#include <gfx/vga_colors.h>

#include <dev/serial.h>

#include <lib/mutex.h>
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

    uint32_t mutex_lock;
} VgaWriter;

static VgaWriter writer = {
    .buffer = (VgaBuffer *)0xb8000,
    .col = 0,
    .row = VGA_ROWS - 1,
    .color = VGA_COLOR_WHITE,
    .previous_color = VGA_COLOR_WHITE,
    .mutex_lock = MUTEX_UNLOCKED,
};

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

static void vga_puts(char *str)
{
    while (*str)
    {
        vga_putchar(*str++);
    }
}

// Public methods

void vga_setcolor(char color_code)
{
    spin_lock(&writer.mutex_lock);
    writer.previous_color = writer.color;
    writer.color = color_code;
    spin_unlock(&writer.mutex_lock);
}

void vga_restore_color()
{
    char temp = writer.color;

    spin_lock(&writer.mutex_lock);
    writer.color = writer.previous_color;
    writer.previous_color = temp;
    spin_unlock(&writer.mutex_lock);
}

void vga_putchar(char ch)
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
}


void vga_printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    SString ss = ss_vprintf(fmt, args);

    spin_lock(&writer.mutex_lock);
    vga_puts(ss.data);
    spin_unlock(&writer.mutex_lock);

    va_end(args);
}