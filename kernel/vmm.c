#include "paging.h"
#include "vmm.h"

void vmm_init(void)
{
}

static void vmm_invalidate_page(uint32_t virtual_addr)
{
    __asm__ volatile("invlpg (%0)" :: "r"((void *)virtual_addr) : "memory");
}

void vmm_map_page(uint32_t virtual_addr, uint32_t physical_addr, uint32_t flags)
{
    uint32_t *page_table = paging_get_first_table();
    uint32_t page_index = virtual_addr / 0x1000;

    if (page_index >= 1024)
    {
        return;
    }

    page_table[page_index] = (physical_addr & 0xFFFFF000) | flags;
    vmm_invalidate_page(virtual_addr);
}

void vmm_unmap_page(uint32_t virtual_addr)
{
    uint32_t *page_table = paging_get_first_table();
    uint32_t page_index = virtual_addr / 0x1000;

    if (page_index >= 1024)
    {
        return;
    }

    page_table[page_index] = 0;
    vmm_invalidate_page(virtual_addr);
}