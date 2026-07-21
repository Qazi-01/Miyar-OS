#ifndef PMM_H
#define PMM_H

#include <stdint.h>

void pmm_init(void);
uint64_t pmm_total_memory(void);
uint64_t pmm_usable_memory(void);
uint32_t pmm_total_frames(void);

void *pmm_alloc_frame(void);
void pmm_free_frame(void *frame);

#endif