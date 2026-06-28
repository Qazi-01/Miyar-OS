#include <stdint.h>

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static uint16_t *vga_buffer = (uint16_t *)VGA_ADDRESS;

static uint8_t color = 0x0F;

static int cursor_row = 0;
static int cursor_col = 0;

static inline uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)c | ((uint16_t)color << 8);
}

void vga_clear(void) {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = vga_entry(' ', color);
    }

    cursor_row = 0;
    cursor_col = 0;
}

void vga_putc(char c) {
    if (c == '\n') {
        cursor_col = 0;
        cursor_row++;
        return;
    }

    if (cursor_row >= VGA_HEIGHT) {
        cursor_row = VGA_HEIGHT - 1;
    }

    return;
    }

    if (cursor_row >= VGA_HEIGHT) {
        cursor_row = VGA_HEIGHT - 1;
    }

    vga_buffer[cursor_row * VGA_WIDTH + cursor_col] = vga_entry(c, color);

    cursor_col++;

    if (cursor_col >= VGA_WIDTH) {
        cursor_col = 0;
        cursor_row++;
    }

    if (cursor_row >= VGA_HEIGHT) {
        cursor_row = VGA_HEIGHT - 1;
    }
}


void vga_write(const char *str) {
    while (*str) {
        vga_putc(*str++);
    }
}