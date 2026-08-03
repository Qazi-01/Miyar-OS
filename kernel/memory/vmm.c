#include "memory/paging.h"
#include "memory/pmm.h"
#include "memory/vmm.h"

static uint32_t *page_tables[1024];

void vmm_init(void)
{
    page_tables[0] = paging_get_first_table();

    for (uint32_t i = 1; i < 1024; i++)
    {
        page_tables[i] = 0;
    }
}

static void vmm_invalidate_page(uint32_t virtual_addr)
{
    __asm__ volatile("invlpg (%0)" :: "r"((void *)virtual_addr) : "memory");
}

static uint32_t *vmm_create_page_table(uint32_t directory_index)
{
    uint32_t *page_table = (uint32_t *)pmm_alloc_frame();

    if (page_table == 0)
    {
        return 0;
    }

    for (uint32_t i = 0; i < 1024; i++)
    {
        page_table[i] = 0;
    }

    page_tables[directory_index] = page_table;
    paging_get_directory()[directory_index] = ((uint32_t)page_table) | PAGE_PRESENT | PAGE_WRITABLE;
    return page_table;
}

void vmm_map_page(uint32_t virtual_addr, uint32_t physical_addr, uint32_t flags)
{
    uint32_t directory_index = virtual_addr >> 22;
    uint32_t table_index = (virtual_addr >> 12) & 0x3FF;

    uint32_t *page_table = page_tables[directory_index];

    if (page_table == 0)
    {
        page_table = vmm_create_page_table(directory_index);
        
        if (page_table == 0)
        {
            return;
        }
    }

    page_table[table_index] = (physical_addr & 0xFFFFF000) | flags;
    vmm_invalidate_page(virtual_addr);
}

void vmm_unmap_page(uint32_t virtual_addr)
{
    uint32_t directory_index = virtual_addr >> 22;
    uint32_t table_index = (virtual_addr >> 12) & 0x3FF;

    uint32_t *page_table = page_tables[directory_index];

    if (page_table == 0)
    {
        return;
    }

    page_table[table_index] = 0;
    vmm_invalidate_page(virtual_addr);
}