#include <stdint.h>

#include "keyboard.h"
#include "serial.h"
#include "terminal.h"
#include "vga.h"

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
