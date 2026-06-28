#include <stdint.h>

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static uint16_t *vga_buffer = (uint16_t *)VGA_ADDRESS;

static uint8_t color = 0x0F;

static int cursor = 0;

static inline uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)c | ((uint16_t)color << 8);
}

void vga_clear(void) {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = vga_entry(' ', color);
    }

    cursor = 0;
}

void vga_putc(char c) {
    if (cursor >= VGA_WIDTH * VGA_HEIGHT)
        return;

    vga_buffer[cursor++] = vga_entry(c, color);
}

void vga_write(const char *str) {
    while (*str) {
        vga_putc(*str++);
    }
}