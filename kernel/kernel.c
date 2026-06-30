#include <stdint.h>

#include "keyboard.h"
#include "serial.h"
#include "terminal.h"
#include "vga.h"

static const char scancode_table[128] = {0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', '0', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ',};

__attribute__((used))
void kernel_main(uint32_t magic, uint32_t *multiboot_info) {
    (void)magic;
    (void)multiboot_info;

    serial_init();
    keyboard_init();

    terminal_init();

    terminal_write("Welcome!\n");
    terminal_write("System boot successful.\n");
    terminal_write("\n");
    terminal_prompt();

    while (1) {
        __asm__ volatile ("hlt");
    }
}