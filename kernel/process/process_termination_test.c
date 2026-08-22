#include "process/process_termination_test.h"
#include "process/process.h"
#include "process/thread.h"
#include "terminal.h"
#include "drivers/timer.h"

#include <stdint.h>

static volatile uint32_t process_termination_runs = 0;

static void process_termination_thread(void)
{
    terminal_writeIn("Process termination test thread: running.");
    process_termination_runs++;
    process_exit(process_current());

    while (1)
    {
        thread_yield();
    }
}

void process_termination_test(void)
{
    terminal_writeIn("Process termination test started.");
    process_t *process = process_create("Termination");

    if (process == 0)
    {
        terminal_writeIn("Process termination test: process creation failed.");
        return;
    }

    thread_t *thread = thread_create_in_process(process, process_termination_thread, "Termination Thread");

    if (thread == 0)
    {
        terminal_writeIn("Process termination test: thread creation failed.");
        process_destroy(process);
        return;
    }

    thread_enqueue(thread);
    terminal_writeIn("Process termination test: thread queued.");
    uint32_t start_ticks = timer_ticks();

    while (timer_ticks() - start_ticks < 50)
    {
        __asm__ volatile("hlt");
    }

    terminal_writeIn("Process termination test: timer wait completed.");

    if (process_termination_runs == 0)
    {
        terminal_writeIn("Process termination test: thread never ran.");
        return;
    }

    terminal_writeIn("Process termination test: thread ran.");

    terminal_writeIn("========================================");
    terminal_writeIn("    PROCESS TERMINATION TEST: PASSED");
    terminal_writeIn("========================================");
}