#include <stdint.h>

#include "gdt.h"
#include "idt.h"
#include "keyboard.h"
#include "pic.h"
#include "serial.h"
#include "shell.h"
#include "timer.h"
#include "terminal.h"
#include "vga.h"

__attribute__((used))
void kernel_main(uint32_t magic, uint32_t *multiboot_info) {
    (void)magic;
    (void)multiboot_info;

    serial_init();
    terminal_init();
    terminal_writeIn("============================================================\n");
    terminal_writeIn("                        MIYAR OS");
    terminal_writeIn("                          v0.1\n");
    terminal_writeIn("============================================================\n");
    terminal_writeIn("");
    terminal_writeIn("Booting kernel...");

    gdt_init();
    terminal_writeIn("Loading GDT....................... [ OK ]");

    idt_init();
    terminal_writeIn("Loading IDT....................... [ OK ]");

    pic_init();
    terminal_writeIn("Initializing PIC................. [ OK ]");

    keyboard_init();
    terminal_writeIn("Initializing Keyboard............ [ OK ]");

    timer_init();
    terminal_writeIn("Initializing Terminal............ [ OK ]");

    __asm__ volatile("sti");

    terminal_writeIn("");
    terminal_writeIn("");
    terminal_writeIn("System initialized successfully.");
    terminal_writeIn("Type \"help\" to get started.");
    terminal_writeIn("");
    terminal_prompt();

    char input[128];
    int index = 0;

    while (1) {
        char c = keyboard_getchar();

        if (!c)
        {
            continue;
        }

        if (c == '\b')
        {
            if (index > 0)
            {
                index--;
                terminal_backspace();
            }

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
