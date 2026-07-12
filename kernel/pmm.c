#include "pmm.h"
#include "memory_map.h"

#define PAGE_SIZE 4096

static uint32_t total_frames = 0;
static uint64_t total_memory = 0;
static uint64_t usable_memory = 0;

void pmm_init(void)
{
    total_memory = 0;
    usable_memory = 0;

    for (uint32_t i = 0; i < memory_region_count; i++)
    {
        total_memory += memory_regions[i].length;

        if (memory_regions[i].type == 1)
        {
            usable_memory += memory_regions[i].length;
        }
    }

    total_frames = usable_memory/PAGE_SIZE;
}

uint64_t pmm_total_memory(void)
{
    return total_memory;
}

uint64_t pmm_usable_memory(void)
{
    return usable_memory;
}

uint32_t pmm_total_frames(void)
{
    return total_frames;
}