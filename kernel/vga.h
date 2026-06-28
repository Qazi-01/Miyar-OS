#ifndef VGA_H
#define VGA_H

void vga_clear(void);
void vga_putc(char c);
void vga_write(const char *str);
void vga_set_color(uint8_t foreground, uint8_t background);

#endif