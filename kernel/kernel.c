#include <stdint.h>

#include "arch/x86/gdt.h"
#include "memory/heap.h"
#include "arch/x86/idt.h"
#include "drivers/keyboard.h"
#include "memory/multiboot.h"
#include "memory/paging.h"
#include "drivers/pic.h"
#include "memory/pmm.h"
#include "drivers/serial.h"
#include "shell.h"
#include "drivers/timer.h"
#include "terminal.h"
#include "drivers/vga.h"
#include "drivers/ata.h"
#include "drivers/disk.h"

#define MULTIBOOT_BOOTLOADER_MAGIC 0x2BADB002

__attribute__((used))
void kernel_main(uint32_t magic, multiboot_info_t *multiboot_info) {
    
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
    heap_init();
    paging_init();

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

    ata_init();
    terminal_writeIn("Initializing ATA.................. [ OK ]");
    ata_detect_devices();
    disk_init();

    const disk_t *disk = disk_get(0);

    if (disk)
    {
        terminal_writeIn("ATA Drive......................... [ OK ]");
    }
    
    else
    {
        terminal_writeIn("ATA Drive......................... [FAIL]");
    }

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
