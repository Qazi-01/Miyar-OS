#include "pmm.h"
#include "memory_map.h"

#define PAGE_SIZE 4096
#define MAX_MEMORY_BYTES (4ULL * 1024 * 1024 * 1024)
#define MAX_FRAMES (MAX_MEMORY_BYTES/PAGE_SIZE)

static uint32_t total_frames = 0;
static uint64_t total_memory = 0;
static uint64_t usable_memory = 0;
static void bitmap_clear(void);
static void bitmap_set(uint32_t frame);
static void bitmap_reset(uint32_t frame);
static int bitmap_test(uint32_t frame);
static void reserve_region(uint64_t base, uint64_t length)

void pmm_init(void)
{
    bitmap_clear();
    
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

    for (uint32_t i = 0; i < memory_region_count; i++)
    {
        if (memory_regions[i].type != 1)
        {
            reserve_region(memory_regions[i].base, memory_regions[i].length);
        }
    }
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

static uint8_t frame_bitmap[MAX_FRAMES/8];

static void bitmap_clear(void)
{
    for (uint32_t i = 0; i < sizeof(frame_bitmap); i++)
    {
        frame_bitmap[i] = 0;
    }
}

static void bitmap_set(uint32_t frame)
{
    frame_bitmap[frame/8] |= (1 << (frame % 8));
}

static void bitmap_reset(uint32_t frame)
{
    frame_bitmap[frame/8] &= ~(1 << (frame % 8));
}

static int bitmap_test(uint32_t frame)
{
    return frame_bitmap[frame/8] & (1 << (frame % 8));
}

static void reserve_region(uint64_t base, uint64_t length)
{
    uint32_t first_frame = base / PAGE_SIZE;
    uint32_t last_frame = (base + length + PAGE_SIZE - 1)/PAGE_SIZE;

    for (uint32_t frame = first_frame; frame < last_frame; frame++)
    {
        bitmap_set(frame);
    }
}