#include "serial.h"
#include "terminal.h"
#include "vga.h"

void terminal_init(void)
{
    vga_clear();

    vga_set_color(VGA_WHITE, VGA_BLACK);
}

void terminal_write(const char *str)
{
    vga_write(str);
    kputs(str);
}