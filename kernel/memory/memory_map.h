#ifndef MEMORY_MAP_H
#define MEMORY_MAP_H

#include <stdint.h>

#define MAX_MEMORY_REGIONS 32

typedef struct
{
    uint64_t base;
    uint64_t length;
    uint64_t type;

} memory_region_t;

extern memory_region_t memory_regions[MAX_MEMORY_REGIONS];
extern uint32_t memory_region_count;

#endif