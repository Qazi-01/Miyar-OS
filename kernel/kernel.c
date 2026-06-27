/* kernel - VGA text mode output */
#include <stdint.h>

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

uint16_t* vga_buffer = (uint16_t*)VGA_ADDRESS;

static inline uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)c | (uint16_t)color << 8;
}

void kernel_main(uint32_t magic, uint32_t* multiboot_info) {
    (void)magic;
    (void)multiboot_info;

    uint8_t color = 0x0F; /* WHITE ON BLACK */
    vga_buffer[0] = vga_entry('M', color);
    vga_buffer[1] = vga_entry('i', color);
    vga_buffer[2] = vga_entry('y', color);
    vga_buffer[3] = vga_entry('a', color);
    vga_buffer[4] = vga_entry('r', color);
    vga_buffer[5] = vga_entry('O', color);
    vga_buffer[6] = vga_entry('S', color);

    for (;;) {
        _asm_; volatile ("hlt");
    }
}