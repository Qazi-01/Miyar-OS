#include <stdint.h>

#include "gdt.h"
#include "idt.h"
#include "keyboard.h"
#include "multiboot.h"
#include "pic.h"
#include "pmm.h"
#include "serial.h"
#include "shell.h"
#include "timer.h"
#include "terminal.h"
#include "vga.h"

__attribute__((used))
void kernel_main(uint32_t magic, multiboot_info_t *multiboot_info) {
    #define MULTIBOOT_BOOTLOADER_MAGIC 0x2BADB002

    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        terminal_init();
        terminal_writeIn("ERROR: Invalid bootloader magic number.");

        while (1){
            __asm__ volatile("hlt");
        }
    }

    if (!(multiboot_info->flags & (1 << 6)))
    {
        terminal_init();
        terminal_writeIn("ERROR: memory map not provided.");
        while (1){
            __asm__ volatile("hlt");
        }
    }

    multiboot_print_memory_map(multiboot_info);
    pmm_init();

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
    terminal_writeIn("Initializing PIC.................. [ OK ]");

    keyboard_init();
    terminal_writeIn("Initializing Keyboard............. [ OK ]");

    timer_init();
    terminal_writeIn("Initializing Timer................ [ OK ]");

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
