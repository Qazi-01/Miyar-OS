#ifndef VMM_H
#define VMM_H

#include <stdint.h>

void vmm_init(void);
void vmm_map_page(uint32_t virtual_addr, uint32_t physical_addr, uint32_t flags);
void vmm_unmap_page(uint32_t virtual_addr);

#endif