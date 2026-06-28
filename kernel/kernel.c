/* MiyarOS kernel - VGA + Serial output */
#include <stdint.h>

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH   80
#define VGA_HEIGHT  25

uint16_t* vga_buffer = (uint16_t*)VGA_ADDRESS;

static inline uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)c | (uint16_t)color << 8;
}

/* Port I/O helpers */
static inline void outb(uint16_t port, uint8_t value) {
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t result;
    __asm__ volatile ("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

/* Write a character to COM1 serial port */
static inline void serial_putc(char c) {
    while ((inb(0x3FD) & 0x20) == 0);
    outb(0x3F8, c);
}

__attribute__((used))
void kernel_main(uint32_t magic, uint32_t* multiboot_info) {
    (void)magic;
    (void)multiboot_info;

    uint8_t color = 0x0F;

    /* VGA output */
    vga_buffer[0] = vga_entry('M', color);
    vga_buffer[1] = vga_entry('i', color);
    vga_buffer[2] = vga_entry('y', color);
    vga_buffer[3] = vga_entry('a', color);
    vga_buffer[4] = vga_entry('r', color);
    vga_buffer[5] = vga_entry('O', color);
    vga_buffer[6] = vga_entry('S', color);

    /* Serial output */
    serial_putc('M');
    serial_putc('i');
    serial_putc('y');
    serial_putc('a');
    serial_putc('r');
    serial_putc('O');
    serial_putc('S');
    serial_putc('\n');

    for (;;) {
        __asm__ volatile ("hlt");
    }
}
