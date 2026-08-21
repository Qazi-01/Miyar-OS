#include "process/scheduler_test.h"
#include "process/thread.h"
#include "terminal.h"

#include <stdint.h>

static volatile uint32_t thread_a_ticks = 0;
static volatile uint32_t thread_b_ticks = 0;

static void scheduler_test_thread_a(void)
{
    while (1)
    {
        thread_a_ticks++;
    }
}

static void scheduler_test_thread_b(void)
{
    while (1)
    {
        thread_b_ticks++;
    }
}

void scheduler_test(void)
{
    terminal_writeIn("Scheduler preemption test started.\n");
    thread_t *thread_a = thread_create(scheduler_test_thread_a, "Scheduler A");

    if (thread_a == 0)
    {
        terminal_writeIn("Scheduler test: failed to create thread A.\n");
        return;
    }

    thread_t *thread_b = thread_create(scheduler_test_thread_b, "Scheduler B");

    if (thread_b == 0)
    {
        terminal_writeIn("Scheduler test: failed to create thread B.\n");
        thread_destroy(thread_a);
        return;
    }

    thread_enqueue(thread_a);
    thread_enqueue(thread_b);

    terminal_writeIn("Scheduler test threads created.");
}