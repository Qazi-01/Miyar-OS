#include "memory/address_space.h"
#include "memory/paging.h"
#include "memory/pmm.h"
#include "memory/vmm.h"

#include <stdint.h>
#include <stdbool.h>

#define USER_SPACE_START 0x00400000
#define KERNEL_SPACE_START 0xC0000000

void vmm_init(void)
{
}

static bool vmm_is_user_address(uint32_t virtual_addr)
{
    return virtual_addr >= USER_SPACE_START && virtual_addr < KERNEL_SPACE_START;
}

static void vmm_invalidate_page(uint32_t virtual_addr)
{
    __asm__ volatile("invlpg (%0)" :: "r"((void *)virtual_addr) : "memory");
}

static uint32_t *vmm_create_page_table(address_space_t *address_space, uint32_t directory_index)
{
    if (address_space == 0)
    {
        return 0;
    }

    uint32_t *page_table = (uint32_t *)pmm_alloc_frame();

    if (page_table == 0)
    {
        return 0;
    }

    for (uint32_t i = 0; i < 1024; i++)
    {
        page_table[i] = 0;
    }

    address_space->page_tables[directory_index] = page_table;
    address_space->page_directory[directory_index] = ((uint32_t)page_table) | PAGE_PRESENT | PAGE_WRITABLE | PAGE_USER;

    return page_table;
}

void vmm_map_page(uint32_t virtual_addr, uint32_t physical_addr, uint32_t flags)
{
    address_space_t *address_space = address_space_current();

    if (address_space == 0)
    {
        return;
    }

    if (!vmm_is_user_address(virtual_addr))
    {
        return;
    }

    uint32_t directory_index = virtual_addr >> 22;
    uint32_t table_index = (virtual_addr >> 12) & 0x3FF;
    uint32_t *page_table = address_space->page_tables[directory_index];

    if (page_table == 0)
    {
        page_table = vmm_create_page_table(address_space, directory_index);
        
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
    address_space_t *address_space = address_space_current();

    if (address_space == 0)
    {
        return;
    }

    uint32_t directory_index = virtual_addr >> 22;
    uint32_t table_index = (virtual_addr >> 12) & 0x3FF;
    uint32_t *page_table = address_space->page_tables[directory_index];

    if (page_table == 0)
    {
        return;
    }

    page_table[table_index] = 0;
    vmm_invalidate_page(virtual_addr);
}

uint32_t vmm_get_physical_address(uint32_t virtual_addr)
{
    address_space_t *address_space = address_space_current();

    if (address_space == 0)
    {
        return 0;
    }

    uint32_t directory_index = virtual_addr >> 22;
    uint32_t table_index = (virtual_addr >> 12) & 0x3FF;
    uint32_t *page_table = address_space->page_tables[directory_index];

    if (page_table == 0)
    {
        return 0;
    }

    uint32_t entry = page_table[table_index];
    
    if (!(entry & PAGE_PRESENT))
    {
        return 0;
    }

    return entry & 0xFFFFF000;
}