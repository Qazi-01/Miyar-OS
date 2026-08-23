#include "process/scheduler_test.h"
#include "process/process.h"
#include "process/thread.h"
#include "drivers/timer.h"
#include "terminal.h"

#include <stdint.h>

static volatile uint32_t thread_a_ticks = 0;
static volatile uint32_t thread_b_ticks = 0;
static volatile uint32_t termination_test_ran = 0;
static volatile uint32_t termination_test_after_terminate = 0;
static volatile uint32_t scheduler_test_started = 0;
static volatile uint32_t scheduler_test_yielded = 0;
static volatile uint32_t scheduler_test_resumed = 0;
static volatile uint32_t scheduler_test_finished = 0;
static thread_t *scheduler_test_worker_thread = 0;

static void scheduler_test_terminating_thread(void)
{
    termination_test_ran = 1;
    terminal_writeIn("Termination test thread: running.");
    thread_terminate();
    termination_test_after_terminate++;

    while (1)
    {
        __asm__ volatile("hlt");
    }
}

static void scheduler_test_worker(void)
{
    scheduler_test_started = 1;
    thread_yield();
    scheduler_test_yielded = 1;

    while (!scheduler_test_finished)
    {
        thread_yield();
    }

    scheduler_test_resumed = 1;
    thread_terminate();
}

static void scheduler_test_thread_a(void)
{
    while (thread_a_ticks < 1000000)
    {
        thread_a_ticks++;
    }

    while (1)
    {
        __asm__ volatile("hlt");
    }
}

static void scheduler_test_thread_b(void)
{
    while (thread_b_ticks < 1000000)
    {
        thread_b_ticks++;
    }

    while (1)
    {
        __asm__ volatile("hlt");
    }
}

void scheduler_test(void)
{
    terminal_writeIn("Scheduler preemption test started.\n");
    process_t *current_process = process_current();

    if (current_process == 0)
    {
        terminal_writeIn("Scheduler test: current process is NULL.");
        return;
    }

    thread_t *current_thread = thread_current();

    if (current_thread == 0)
    {
        terminal_writeIn("Scheduler test: current thread is NULL.");
        return;
    }

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
    terminal_writeIn("Waiting for timer preemption...");
    uint32_t start_ticks = timer_ticks();

    while (timer_ticks() - start_ticks < 50)
    {
        __asm__ volatile("hlt");
    }

    terminal_writeIn("Scheduler test: timer wait completed.");
    terminal_writeIn("Scheduler test: Thread A ran.");
    terminal_writeIn("Scheduler test: Thread B ran.");

    if (thread_a_ticks == 0)
    {
        terminal_writeIn("Scheduler test: Thread A FAILED.");
        return;
    }

    if (thread_b_ticks == 0)
    {
        terminal_writeIn("Scheduler test: Thread B FAILED.");
        return;
    }

    terminal_writeIn("Scheduler test: Thread A executed.");
    terminal_writeIn("Scheduler test: Thread B executed.");

    terminal_writeIn("========================================");
    terminal_writeIn("    SCHEDULER PREEMPTION TEST PASSED");
    terminal_writeIn("========================================");

    terminal_writeIn("\n");

    terminal_writeIn("Termination test started.");
    thread_t *termination_thread = thread_create(scheduler_test_terminating_thread, "Terminator");

    if (termination_thread == 0)
    {
        terminal_writeIn("Termination test: failed to create thread.");
        return;
    }

    thread_enqueue(termination_thread);
    terminal_writeIn("Termination test: thread created");

    while (termination_test_ran == 0)
    {
        if (timer_ticks() - start_ticks > 200)
        {
            terminal_writeIn("Termination test: thread never ran.");
            return;
        }

        __asm__ volatile("hlt");
    }

    terminal_writeIn("Termination test: thread ran.");
    uint32_t termination_ticks = timer_ticks();

    while (timer_ticks() - termination_ticks < 50)
    {
        __asm__ volatile("hlt");
    }

    if (termination_thread->state != THREAD_TERMINATED)
    {
        terminal_writeIn("Termination test: thread termination FAILED.");
        return;
    }

    terminal_writeIn("Termination test: PASSED.");
}

void scheduler_test_yield(void)
{
    terminal_writeIn("Scheduler invariant test started.");

    scheduler_test_started = 0;
    scheduler_test_yielded = 0;
    scheduler_test_resumed = 0;
    scheduler_test_finished = 0;
    scheduler_test_worker_thread = 0;

    thread_t *worker = thread_create(scheduler_test_worker, "Scheduler Test");

    if (worker == 0)
    {
        terminal_writeIn("Scheduler test: thread creation FAILED.");
        return;
    }

    scheduler_test_worker_thread = worker;

    if (worker->state != THREAD_READY)
    {
        terminal_writeIn("Scheduler test: new thread is not READY.");
        thread_destroy(worker);
        scheduler_test_worker_thread = 0;
        return;
    }

    terminal_writeIn("New thread state: READY");
    thread_enqueue(worker);

    while (!scheduler_test_started)
    {
        thread_yield();
    }

    if (thread_current() == 0)
    {
        terminal_writeIn("Scheduler test: current thread is NULL.");
        return;
    }

    if (thread_current()->state != THREAD_RUNNING)
    {
        terminal_writeIn("Scheduler test: current thread is NOT RUNNING.");
        return;
    }

    terminal_writeIn("Current thread state: RUNNING.");

    while (!scheduler_test_yielded)
    {
        thread_yield();
    }

    terminal_writeIn("Thread yield completed.");

    if (worker->state == THREAD_RUNNING)
    {
        terminal_writeIn("Scheduler test: yielded thread remained RUNNING.");
        return;
    }

    if (worker->state != THREAD_READY)
    {
        terminal_writeIn("Scheduler test: yielded thread is NOT READY.");
        return;
    }

    terminal_writeIn("Yielded thread state: READY.");
    scheduler_test_finished = 1;

    while (!scheduler_test_resumed)
    {
        thread_yield();
    }

    terminal_writeIn("Thread resumed successfully.");

    if (worker->state != THREAD_TERMINATED)
    {
        terminal_writeIn("Scheduler test: Thread termination FAILED.");
        return;
    }

    terminal_writeIn("Scheduler test: Thread TERMINATED.");

    terminal_writeIn("========================================");
    terminal_writeIn("    SCHEDULER INVARIANT TEST: PASSED");
    terminal_writeIn("========================================");
}