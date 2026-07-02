#include <stdint.h>

#include "idt.h"
#include "keyboard.h"
#include "pic.h"
#include "serial.h"
#include "shell.h"
#include "terminal.h"
#include "vga.h"

__attribute__((used))
void kernel_main(uint32_t magic, uint32_t *multiboot_info) {
    (void)magic;
    (void)multiboot_info;

    serial_init();
    keyboard_init();
    terminal_init();
    pic_init();
    idt_init();

    __asm__ volatile("sti");

    terminal_write("Welcome!\n");
    terminal_write("System boot successful.\n");
    terminal_write("\n");
    terminal_prompt();

    char input[128];
    int index = 0;

    while (1) {
        char c = keyboard_getchar();

        if (!c)
        {
            continue;
        }

        if (c == '\n')
        {
            input[index] = '\0';

            shell_execute(input);
            terminal_prompt();
            
            index = 0;
            continue;
        }

        if (index < 127)
        {
            input[index++] = c;

            terminal_write((char[]){c, '\0'});
        }
    }
}
