#include "serial.h"
#include "terminal.h"
#include "vga.h"

void terminal_init(void)
{
    vga_set_color(VGA_BLACK, VGA_WHITE);

    vga_clear();

    terminal_writeIn("====================");
    terminal_writeIn("MiyarOS v0.1\n");
    terminal_writeIn("====================");
    terminal_writeIn("Initializing Terminal... OK\n");
    terminal_writeIn("\n");
}

void terminal_write(const char *str)
{
    vga_write(str);
    kputs(str);
}

void terminal_writeIn(const char *str)
{
    terminal_write(str);
    terminal_write("\n");
}

void terminal_prompt(void)
{
    terminal_write("> ");
}

char terminal_readchar(void)
{
    return 0;
}

void terminal_clear(void)
{
    vga_clear();
}

void terminal_backspace(void)
{
}
