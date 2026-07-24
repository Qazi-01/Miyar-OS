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
    
    uint8_t sector[512];

    const ata_device_t *disk = ata_get_device(0);

    if (disk && disk->present)
    {
        if (ata_read_sector(disk, 0, sector) == 0)
        {
            terminal_write("First 16 bytes:\n");

            for (int i = 0; i < 16; i++)
            {
                terminal_write_hex(sector[i]);
                terminal_write(" ");
            }
            
            terminal_write("\n");
        }
    }

    uint8_t write_buffer[512];
    uint8_t read_buffer[512];
    
    for (int i = 0; i < 512; i++)
    {
        write_buffer[i] = (uint8_t)i;
        read_buffer[i] = 0;
    }

    if (disk && disk->present)
    {
        if (ata_write_sector(disk, 100, write_buffer) == 0)
        {
            terminal_write("Write successful\n");
        }

        else
        {
            terminal_write("Write failed\n");
        }
        
        if (ata_read_sector(disk, 100, read_buffer) == 0)
        {
            terminal_write("Read successful\n");

            bool match = true;

            for (int i = 0; i < 512; i++)
            {
                if (write_buffer[i] != read_buffer[i])
                {
                    match = false;
                    break;
                }
            }

            if (match)
            {
                terminal_write("ATA: Read/Write verified\n");
            }
            else
            {
                terminal_write("ATA: verification failed\n");
            }
        }

        else
        {
            terminal_write("Read failed\n");
        }
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
