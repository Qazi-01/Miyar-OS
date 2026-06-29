#include "serial.h"
#include "terminal.h"
#include "vga.h"

void terminal_init(void)
{
    vga_set_color(VGA_BLACK, VGA_WHITE);

    vga_clear();

    terminal_write("====================\n");
    terminal_write("MiyarOS v0.1\n");
    terminal_write("====================\n");
    terminal_write("Initializing Terminal... OK\n");
    terminal_write("\n");
}

void terminal_write(const char *str)
{
    vga_write(str);
    kputs(str);
}