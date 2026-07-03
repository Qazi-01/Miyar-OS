#include "serial.h"
#include "terminal.h"
#include "vga.h"

void terminal_init(void)
{
    vga_set_color(VGA_BLACK, VGA_WHITE);

    vga_clear();

    terminal_writeIn("============================================================\n");
    terminal_writeIn("                        MIYAR OS\n");
    terminal_writeIn("                          v0.1\n");
    terminal_writeIn("============================================================\n");
    terminal_writeIn("");
    terminal_writeIn("Booting kernel...");
    terminal_writeIn("Loading GDT....................... [ OK ]");
    terminal_writeIn("Loading IDT....................... [ OK ]");
    terminal_writeIn("Initializing PIC.................. [ OK ]");
    terminal_writeIn("Initializing Keyboard............. [ OK ]");
    terminal_writeIn("Initializing Terminal............. [ OK ]");
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

void terminal_clear(void)
{
    vga_clear();
}

void terminal_backspace(void)
{
    vga_backspace();
}