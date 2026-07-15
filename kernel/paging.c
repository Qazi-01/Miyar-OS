#include "paging.h"

#include <stdint.h>

extern char _kernel_start;
extern char _kernel_end;

static uint32_t page_directory[1024] __attribute__((aligned(4096)));
static uint32_t first_page_table[1024] __attribute__((aligned(4096)));

static void load_page_directory(uint32_t *page_directory)
{
    __asm__ volatile("mov %0, %%cr3" :: "r"(page_directory) : "memory");
}

static void enable_paging(void)
{
    uint32_t cr0;
    __asm__ volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    __asm__ volatile("mov %0, %%cr0" :: "r"(cr0));
}

void paging_init(void)
{
    for (uint32_t i = 0; i < 1024; i++)
    {
        first_page_table[i] = (i * 0x1000) | PAGE_PRESENT | PAGE_WRITABLE;
    }

    for (uint32_t i = 0; i < 1024; i++)
{
    first_page_table[i] = (i * 0x1000) | PAGE_PRESENT | PAGE_WRITABLE;
}

uint32_t start_page = (uintptr_t)&_kernel_start / 0x1000;
uint32_t end_page = ((uintptr_t)&_kernel_end + 0xFFF) / 0x1000;

for (uint32_t i = start_page; i < end_page; i++)
{
    first_page_table[i] &= ~PAGE_WRITABLE;
}

page_directory[0] = ((uint32_t)first_page_table) | PAGE_PRESENT | PAGE_WRITABLE;

    page_directory[0] = ((uint32_t)first_page_table) | PAGE_PRESENT | PAGE_WRITABLE;

    for (uint32_t i = 1; i < 1024; i++)
    {
        page_directory[i] = 0;
    }

    load_page_directory(page_directory);
    enable_paging();
}

uint32_t *paging_get_directory(void)
{
    return page_directory;
}

uint32_t *paging_get_first_table(void)
{
    return first_page_table;
}
