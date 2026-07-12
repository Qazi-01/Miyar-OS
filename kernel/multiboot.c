#include "multiboot.h"

multiboot_memory_map_t *multiboot_mmap_first(multiboot_info_t *mbi)
{
    return (multiboot_memory_map_t *)(uintptr_t)mbi->mmap_addr;
}

multiboot_memory_map_t *multiboot_mmap_next(multiboot_memory_map_t *entry)
{
    return (multiboot_memory_map_t *)((uint8_t *)entry + entry->size + sizeof(entry->size));
}