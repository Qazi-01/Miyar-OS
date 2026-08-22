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

    terminal_writeIn("Checking current process before activation.");

    if (process_current() == 0)
    {
        terminal_writeIn("process_current() = NULL");
    }
    
    else
    {
        terminal_writeIn("process_current() = NOT NULL");
    }

    address_space_activate(process_a->address_space);

    if (process_current() == 0)
    {
        terminal_writeIn("After A activation: CURRENT PROCESS NULL.");
    }
    
    else
    {
        terminal_writeIn("After A activation: CURRENT PROCESS OK.");
    }

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

    terminal_writeIn("Checking current process before activation.");

    if (process_current() == 0)
    {
        terminal_writeIn("process_current() = NULL");
    }
    
    else
    {
        terminal_writeIn("process_current() = NOT NULL");
    }

    address_space_activate(process_b->address_space);

    if (process_current() == 0)
    {
        terminal_writeIn("After B activation: CURRENT PROCESS NULL.");
    }
    
    else
    {
        terminal_writeIn("After B activation: CURRENT PROCESS OK.");
    }

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

    terminal_writeIn("Checking current process before activation.");

    if (process_current() == 0)
    {
        terminal_writeIn("process_current() = NULL");
    }
    
    else
    {
        terminal_writeIn("process_current() = NOT NULL");
    }

    address_space_activate(process_a->address_space);

    if (process_current() == 0)
    {
        terminal_writeIn("After A restore: CURRENT PROCESS NULL.");
    }
    
    else
    {
        terminal_writeIn("After A restore: CURRENT PROCESS OK.");
    }

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

    terminal_writeIn("Checking process list...");
    process_t *list = process_current();

    if (list == 0)
    {
        terminal_writeIn("process_current() = NULL");
    }
    
    else
    {
        terminal_writeIn("process_current() = NOT NULL");
    }

    if (process_a != 0)
    {
        terminal_writeIn("Process A still exists.");
    }

    if (process_b != 0)
    {
        terminal_writeIn("Process B still exists.");
    }

    process_t *kernel_process = process_find(0);

    if (kernel_process == 0)
    {
        terminal_writeIn("ERROR: Kernel process not found.");
    }

    else
    {
        terminal_writeIn("Kernel process restored.");
        address_space_activate(kernel_process->address_space);
        process_set_current(kernel_process);
    }

    if (kernel_process != 0)
    {
        address_space_activate(kernel_process->address_space);
        process_set_current(kernel_process);
    }

    address_space_activate(address_space_kernel());
    pmm_free_frame(frame_a);
    pmm_free_frame(frame_b);

    process_destroy(process_a);
    process_destroy(process_b);
}