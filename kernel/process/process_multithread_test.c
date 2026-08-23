#include "process/process_multithread_test.h"
#include "process/process.h"
#include "process/thread.h"
#include "terminal.h"
#include "drivers/timer.h"

#include <stdint.h>

static volatile uint32_t thread_a_runs = 0;
static volatile uint32_t thread_b_runs = 0;
static volatile uint32_t terminate_thread_b = 0;

static void process_thread_a(void)
{
    terminal_writeIn("Multi-thread process: Thread A running.");
    thread_a_runs++;
    thread_terminate();

    while (1)
    {
        thread_yield();
    }
}

static void process_thread_b(void)
{
    terminal_writeIn("Multi-thread process: Thread B running.");
    thread_b_runs++;

    while (!terminate_thread_b)
    {
        thread_yield();
    }

    terminal_writeIn("Multi-thread process: Thread B terminating.");
    process_exit(thread_current()->process);

    while (1)
    {
        thread_yield();
    }
}

void process_multithread_test(void)
{
    thread_a_runs = 0;
    thread_b_runs = 0;
    terminate_thread_b = 0;

    terminal_writeIn("Multi-thread process test started.");
    process_t *process = process_create("Multi-thread");

    if (process == 0)
    {
        terminal_writeIn("Multi-thread process test: process creation failed.");
        return;
    }

    uint32_t pid = process->pid;
    thread_t *thread_a = thread_create_in_process(process, process_thread_a, "Thread A");
    thread_t *thread_b = thread_create_in_process(process, process_thread_b, "Thread B");

    if (thread_a == 0 || thread_b == 0)
    {
        terminal_writeIn("Multi-thread process test: thread creation failed.");
        return;
    }

    thread_enqueue(thread_a);
    thread_enqueue(thread_b);
    terminal_writeIn("Multi-thread process test: threads queued.");
    uint32_t start_ticks = timer_ticks();

    while (timer_ticks() - start_ticks < 50)
    {
        __asm__ volatile("hlt");
    }

    terminal_writeIn("Multi-thread process test: timer wait completed.");

    if (thread_a_runs == 0)
    {
        terminal_writeIn("Multi-thread process test: Thread A did not run.");
        return;
    }

    if (thread_b_runs == 0)
    {
        terminal_writeIn("Multi-thread process test: Thread B did not run.");
        return;
    }

    terminal_writeIn("Multi-thread process test: both threads ran succesfully.");
    terminate_thread_b = 1;
    terminal_writeIn("Multi-thread process test: requesting Thread B termination.");
    uint32_t terminate_start = timer_ticks();
    uint32_t saw_zero_threads = 0;
    uint32_t reaped_early = 0;

    while (1)
    {
        process_t *found = process_find(pid);

        if (found == 0)
        {
            reaped_early = 1;
            break;
        }

        if (found->thread_count == 0)
        {
            saw_zero_threads = 1;
            break;
        }

        if (timer_ticks() - terminate_start >= 20)
        {
            terminal_writeIn("Multi-thread process test: Threads did not terminate.");
            return;
        }

        __asm__ volatile("hlt");
    }

    if (!saw_zero_threads && !reaped_early)
    {
        terminal_writeIn("Multi-thread process test: teardown state was not observed.");
        return;
    }

    if (saw_zero_threads)
    {
        terminal_writeIn("Multithread process test: all threads terminated succesfully.");
    }

    else
    {
        terminal_writeIn("Multi-thread process test: process reaped before zero-thread observation.");
    }

    uint32_t reap_start = timer_ticks();

    while (!reaped_early && process_find(pid) != 0)
    {
        if (timer_ticks() - reap_start >= 20)
        {
            terminal_writeIn("Multi-thread process test: process was not reaped");
            return;
        }

        __asm__ volatile("hlt");
    }

    terminal_writeIn("Multi-thread process test: process reaped succesfully.");

    terminal_writeIn("=========================================");
    terminal_writeIn("    MULTI-THREAD PROCESS TEST: PASSED");
    terminal_writeIn("=========================================");
}