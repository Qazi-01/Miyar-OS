#include "memory/address_space.h"
#include "memory/pmm.h"
#include "memory/paging.h"
#include "memory/heap.h"

#include <stdint.h>

static address_space_t *current_address_space = 0;
static address_space_t kernel_address_space;
static uint32_t *kernel_page_tables[1024];

static void load_page_directory(uint32_t *page_directory)
{
    __asm__ volatile("mov %0, %%cr3" : : "r"(page_directory) : "memory");
}

address_space_t *address_space_create(void)
{
    address_space_t *address_space = (address_space_t *)pmm_alloc_frame();

    if (address_space == 0)
    {
        return 0;
    }

    address_space->page_tables = (uint32_t **)kmalloc(sizeof(uint32_t *) * 1024);

    if (address_space->page_tables == 0)
    {
        pmm_free_frame(address_space);
        return 0;
    }

    for (uint32_t i = 0; i < 1024; i++)
    {
        address_space->page_tables[i] = 0;
    }

    uint32_t *page_directory = (uint32_t *)pmm_alloc_frame();

    if (page_directory == 0)
    {
        kfree(address_space->page_tables);
        pmm_free_frame(address_space);
        return 0;
    }

    uint32_t *kernel_page_directory = paging_get_directory();

    for (uint32_t i = 0; i < 1024; i++)
    {
        page_directory[i] = 0;
    }

    page_directory[0] = kernel_page_directory[0];
    address_space->page_tables[0] = paging_get_first_table();
    address_space->page_directory = page_directory;

    return address_space;
}

void address_space_destroy(address_space_t *address_space)
{
    if (address_space == 0)
    {
        return;
    }

    if (address_space == current_address_space)
    {
        return;
    }

    if (address_space->page_directory != 0)
    {
        pmm_free_frame(address_space->page_directory);
    }

    if (address_space->page_tables != 0)
    {
        kfree(address_space->page_tables);
    }
    pmm_free_frame(address_space);
}

address_space_t *address_space_current(void)
{
    return current_address_space;
}

void address_space_activate(address_space_t *address_space)
{
    if (address_space == 0)
    {
        return;
    }

    load_page_directory(address_space->page_directory);
    current_address_space = address_space;
}

void address_space_init(void)
{
    kernel_address_space.page_directory = paging_get_directory();
    current_address_space = &kernel_address_space;

    for (uint32_t i = 0; i < 1024; i++)
    {
        kernel_page_tables[i] = 0;
    }

    kernel_page_tables[0] = paging_get_first_table();
    current_address_space = &kernel_address_space;
}