#include "memory/memory_map.h"
#include "memory/multiboot.h"
#include "terminal.h"

void multiboot_print_memory_map(multiboot_info_t *mbi)
{
    multiboot_memory_map_t *entry = multiboot_mmap_first(mbi);
    uint32_t end = mbi->mmap_addr + mbi->mmap_length;
    
    memory_region_count = 0;

    while ((uint32_t)(uintptr_t)entry < end && memory_region_count < MAX_MEMORY_REGIONS)
    {
        memory_regions[memory_region_count].base = entry->addr;
        memory_regions[memory_region_count].length = entry->len;
        memory_regions[memory_region_count].type = entry->type;

        memory_region_count++;
        entry = multiboot_mmap_next(entry);
    }

    // Memory map parsed successfully.
}

multiboot_memory_map_t *multiboot_mmap_first(multiboot_info_t *mbi)
{
    return (multiboot_memory_map_t *)(uintptr_t)mbi->mmap_addr;
}

multiboot_memory_map_t *multiboot_mmap_next(multiboot_memory_map_t *entry)
{
    return (multiboot_memory_map_t *)((uint8_t *)entry + entry->size + sizeof(entry->size));
}