#include "serial.h"
#include "terminal.h"
#include "vga.h"

void terminal_init(void)
{
    vga_set_color(VGA_BLACK, VGA_WHITE);

    vga_clear();

    terminal_write("====================\n");
    terminal_write("MiyarOS\n");
    terminal_write("====================\n\n");
}

void terminal_write(const char *str)
{
    vga_write(str);
    kputs(str);
}