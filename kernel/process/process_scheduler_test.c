#include "process/process_scheduler_test.h"
#include "process/process.h"
#include "process/thread.h"
#include "terminal.h"
#include "drivers/timer.h"

#include <stdint.h>

static volatile uint32_t process_a_runs = 0;
static volatile uint32_t process_b_runs = 0;

static void process_a_thread(void)
{
    while (1)
    {
        process_a_runs++;
    }
}

static void process_b_thread(void)
{
    while (1)
    {
        process_b_runs++;
    }
}

void process_scheduler_test(void)
{
    terminal_writeIn("Multi-process scheduler test started.");
    process_t *process_a = process_create("Scheduler A");
    process_t *process_b = process_create("Scheduler B");

    if (process_a == 0 || process_b == 0)
    {
        terminal_writeIn("Multi-process scheduler test: process creation failed.");
        return;
    }

    thread_t *thread_a = thread_create_in_process(process_a, process_a_thread, "Process A");
    thread_t *thread_b = thread_create_in_process(process_b, process_b_thread, "Process B");

    if (thread_a == 0 || thread_b == 0)
    {
        terminal_writeIn("Multi-process scheduler test: thread creation failed.");
        return;
    }

    thread_enqueue(thread_a);
    thread_enqueue(thread_b);
    terminal_writeIn("Multi-process scheduler test: threads queued.");
    uint32_t start_ticks = timer_ticks();

    while (timer_ticks() - start_ticks < 50)
    {
        __asm__ volatile("hlt");
    }

    terminal_writeIn("Multi-process scheduler test: timer wait completed.");

    if (process_a_runs == 0)
    {
        terminal_writeIn("Multi-process scheduler test: Process A did not run.");
        return;
    }

    if (process_b_runs == 0)
    {
        terminal_writeIn("Multi-process scheduler test: Process B did not run.");
        return;
    }

    terminal_writeIn("Multi-process scheduler test: Both processes ran.");

    terminal_writeIn("========================================");
    terminal_writeIn("  MULTI-PROCESS SCHEDULER TEST: PASSED");
    terminal_writeIn("========================================");
}