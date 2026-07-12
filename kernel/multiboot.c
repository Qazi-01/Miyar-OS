#include "multiboot.h"
#include "terminal.h"

void multiboot_print_memory_map(multiboot_info_t *mbi)
{
    multiboot_memory_map_t *entry = multiboot_mmap_first(mbi);
    uint32_t end = mbi->mmap_addr + mbi->mmap_length;
    int count = 0;

    while ((uint32_t)entry < end)
    {
        count++;
        entry = multiboot_mmap_next(entry);
    }

    terminal_writeIn("Memory map detected.");
}

multiboot_memory_map_t *multiboot_mmap_first(multiboot_info_t *mbi)
{
    return (multiboot_memory_map_t *)(uintptr_t)mbi->mmap_addr;
}

multiboot_memory_map_t *multiboot_mmap_next(multiboot_memory_map_t *entry)
{
    return (multiboot_memory_map_t *)((uint8_t *)entry + entry->size + sizeof(entry->size));
}