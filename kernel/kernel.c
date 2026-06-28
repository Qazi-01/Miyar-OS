/* MiyarOS kernel – VGA + Serial, with init */
#include <stdint.h>

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH   80
#define VGA_HEIGHT  25

uint16_t* vga_buffer = (uint16_t*)VGA_ADDRESS;

static inline uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)c | (uint16_t)color << 8;
}

/* ---- Port I/O ---- */
static inline void outb(uint16_t port, uint8_t value) {
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t result;
    __asm__ volatile ("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

/* ---- Serial (COM1, 0x3F8) ---- */
static void serial_init(void) {
    outb(0x3F8 + 1, 0x00);    /* Disable all interrupts */
    outb(0x3F8 + 3, 0x80);    /* Enable DLAB (set baud rate divisor) */
    outb(0x3F8 + 0, 0x03);    /* Set divisor to 3 (lo byte) 38400 baud */
    outb(0x3F8 + 1, 0x00);    /*                  (hi byte) */
    outb(0x3F8 + 3, 0x03);    /* 8 bits, no parity, one stop bit */
    outb(0x3F8 + 2, 0xC7);    /* Enable FIFO, clear them, with 14-byte threshold */
    outb(0x3F8 + 4, 0x0B);    /* IRQs enabled, RTS/DSR set */
}

static void serial_putc(char c) {
    while ((inb(0x3F8 + 5) & 0x20) == 0);
    outb(0x3F8, c);
}

static void kputs(const char *s) {
    while (*s) {
        serial_putc(*s++);
    }
}

/* ---- Main ---- */
__attribute__((used))
void kernel_main(uint32_t magic, uint32_t* multiboot_info) {
    (void)magic;
    (void)multiboot_info;

    serial_init();   /* <-- MUST be called first */

    uint8_t color = 0x0F;

    /* VGA */
    vga_buffer[0] = vga_entry('M', color);
    vga_buffer[1] = vga_entry('i', color);
    vga_buffer[2] = vga_entry('y', color);
    vga_buffer[3] = vga_entry('a', color);
    vga_buffer[4] = vga_entry('r', color);
    vga_buffer[5] = vga_entry('O', color);
    vga_buffer[6] = vga_entry('S', color);

    /* Serial */
    kputs("MiyarOS\n");

    for (;;) {
        __asm__ volatile ("hlt");
    }
}