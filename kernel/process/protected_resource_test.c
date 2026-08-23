#include "process/protected_resource_test.h"
#include "process/process.h"
#include "memory/address_space.h"
#include "memory/pmm.h"
#include "process/thread.h"
#include "terminal.h"

void protected_resource_test(void)
{
    terminal_writeIn("Protected resource test started.");
    process_t *kernel_process = process_find(0);

    if (kernel_process == 0)
    {
        terminal_writeIn("Protected resources test: kernel process NOT FOUND.");
        return;
    }

    address_space_t *kernel_address_space = kernel_process->address_space;

    if (kernel_address_space == 0)
    {
        terminal_writeIn("Protected resources test: kernel address space missing.");
        return;
    }

    uint32_t frames_before = pmm_free_frames();
    process_destroy(kernel_process);

    if (process_current() != kernel_process)
    {
        terminal_writeIn("Protected resource test: current kernel process changed.");
        return;
    }

    if (kernel_process->state != PROCESS_RUNNING)
    {
        terminal_writeIn("Protected resource test: kernel process state changed.");
        return;
    }

    if (kernel_process->address_space != kernel_address_space)
    {
        terminal_writeIn("Protected resource test: kernel address space changed.");
        return;
    }

    if (process_find(0) != kernel_process)
    {
        terminal_writeIn("Protected resource test: kernel process missing from process list.");
        return;
    }

    if (kernel_process->address_space != kernel_address_space)
    {
        terminal_writeIn("Protected resource test: kernel address space changed.");
        return;
    }

    uint32_t frames_after_destroy = pmm_free_frames();

    if (frames_after_destroy != frames_before)
    {
        terminal_writeIn("Protected resource test: kernel destruction changed PMM.");
        return;
    }

    process_exit(kernel_process);

    if (kernel_process->state != PROCESS_RUNNING)
    {
        terminal_writeIn("Protected resource test: kernel process was terminated.");
        return;
    }

    if (address_space_current() != kernel_address_space)
    {
        terminal_writeIn("Protected resource test: address space changed.");
        return;
    }

    uint32_t frames_after_exit = pmm_free_frames();

    if (frames_after_exit != frames_before)
    {
        terminal_writeIn("Protected resource test: kernel exit changed PMM.");
        return;
    }

    address_space_destroy(kernel_address_space);

    if (process_find(0) == 0)
    {
        terminal_writeIn("Protected resource test: kernel process disappeared.");
        return;
    }

    if (address_space_current() != kernel_address_space)
    {
        terminal_writeIn("Protected resource test: kernel address space destroyed.");
        return;
    }

    uint32_t frames_after_destroy_address_space = pmm_free_frames();

    if (frames_after_destroy_address_space != frames_before)
    {
        terminal_writeIn("Protected resource test: kernel address space destruction changed PMM.");
        return;
    }

    thread_t *bootstrap = thread_current();
    thread_t *idle = thread_idle();

    thread_destroy(bootstrap);
    thread_destroy(idle);

    if (thread_current() != bootstrap)
    {
        terminal_writeIn("Bootstrap thread protection: FAILED.");
        return;
    }

    if (thread_idle() != idle)
    {
        terminal_writeIn("Idle thread protection: FAILED.");
        return;
    }

    terminal_writeIn("Kernel process protection: OK.");
    terminal_writeIn("Kernel address-space protection: OK.");
    terminal_writeIn("Kernel PMM protection: OK.");
    terminal_writeIn("Bootstrap thread protection: OK.");
    terminal_writeIn("Idle thread protection: OK.\n");

    terminal_writeIn("===============================================");
    terminal_writeIn("        PROTECTED RESOURCE TEST: PASSED");
    terminal_writeIn("===============================================");
}