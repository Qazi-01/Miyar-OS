#include "memory/address_space_test.h"
#include "memory/address_space.h"
#include "memory/paging.h"
#include "memory/pmm.h"
#include "process/process.h"
#include "memory/vmm.h"
#include "terminal.h"
#include <stdint.h>

#define ISOLATION_TEST_ADDRESS 0x00400000

void address_isolation_test(void)
{
    terminal_writeIn("Address space isolation test started.");
    process_t *process_a = process_create("Isolation A");
    process_t *process_b = process_create("Isolation B");

    if (process_a == 0 || process_b == 0)
    {
        terminal_writeIn("Isolation test: process creation failed.");
        return;
    }

    if (process_a->address_space == process_b->address_space)
    {
        terminal_writeIn("Isolation test: address spaces FAILED.");
        return;
    }

    if (process_a->address_space->page_directory == process_b->address_space->page_directory)
    {
        terminal_writeIn("Isolation test: page directories FAILED.");
        return;
    }

    void *frame_a = pmm_alloc_frame();
    void *frame_b = pmm_alloc_frame();

    if (frame_a == 0 || frame_b == 0)
    {
        terminal_writeIn("Isolation test: frame allocation failed.");
        return;
    }

    if (frame_a == frame_b)
    {
        terminal_writeIn("Isolation test: frame uniqueness FAILED.");
        return;
    }

    address_space_activate(process_a->address_space);

    if (address_space_current() != process_a->address_space)
    {
        terminal_writeIn("Process A activation FAILED.");
        return;
    }

    vmm_map_page(ISOLATION_TEST_ADDRESS, (uint32_t)frame_a, PAGE_PRESENT | PAGE_WRITABLE | PAGE_USER);

    uint32_t physical_a = vmm_get_physical_address(ISOLATION_TEST_ADDRESS);

    if (physical_a != (uint32_t)frame_a)
    {
        terminal_writeIn("Process A mapping FAILED.");
        return;
    }

    address_space_activate(process_b->address_space);

    if (address_space_current() != process_b->address_space)
    {
        terminal_writeIn("Process B activation FAILED.");
        return;
    }

    uint32_t physical_b = vmm_get_physical_address(ISOLATION_TEST_ADDRESS);

    if (physical_b != 0)
    {
        terminal_writeIn("Isolation test: Process B sees Process A mapping.");
        return;
    }

    vmm_map_page(ISOLATION_TEST_ADDRESS, (uint32_t)frame_b, PAGE_PRESENT | PAGE_WRITABLE | PAGE_USER);
    physical_b = vmm_get_physical_address(ISOLATION_TEST_ADDRESS);

    if (physical_b != (uint32_t)frame_b)
    {
        terminal_writeIn("Process B mapping FAILED.");
    }

    if (physical_a == physical_b)
    {
        terminal_writeIn("Isolation FAILED: mappings share physical frame.");
        return;
    }

    address_space_activate(process_a->address_space);
    uint32_t physical_a_again = vmm_get_physical_address(ISOLATION_TEST_ADDRESS);
    
    if (physical_a_again != (uint32_t)frame_a)
    {
        terminal_writeIn("Isolation FAILED: Process A mapping changed.");
        return;
    }

    terminal_writeIn("Processes created.");
    terminal_writeIn("Seperate physical frames: OK.");
    terminal_writeIn("Seperate address spaces: OK.");
    terminal_writeIn("Process A activated.");
    terminal_writeIn("Process A mapping: OK.");
    terminal_writeIn("Process B activated.");
    terminal_writeIn("Process B cannot see Process A mapping: OK.");
    terminal_writeIn("Process B mapping: OK.");
    terminal_writeIn("Different physical mappings: OK.");
    terminal_writeIn("Process A mapping preserved: OK.");

    terminal_writeIn("========================================");
    terminal_writeIn("PROCESS ADDRESS TEST: PASSED");
    terminal_writeIn("========================================");

    pmm_free_frame(frame_a);
    pmm_free_frame(frame_b);
}