#include "process/process.h"
#include "process/scheduler.h"
#include "process/thread.h"
#include "memory/address_space.h"
#include "terminal.h"

static int scheduler_debugged_idle = 0;

void scheduler_init(void)
{
    thread_init();
}

uint32_t scheduler_schedule(struct registers *r)
{
    thread_t *current = thread_current();
    thread_t *idle = thread_idle();

    if (current == 0)
    {
        return (uint32_t)r;
    }

    current->saved_esp = (uint32_t)r;
    thread_t *next = thread_dequeue();

    if (next == 0)
    {
        next = idle;
    }

    if (next == 0)
    {
        return current->saved_esp;
    }

    if (current != idle && current->state != THREAD_TERMINATED && current->state != THREAD_BLOCKED)
    {
        thread_enqueue(current);
    }
    
    next->state = THREAD_RUNNING;
    thread_set_current(next);

    if (next->process != 0 && current->process != next->process)
    {
        process_set_current(next->process);
        address_space_activate(next->process->address_space);
    }

    thread_reap_terminated(current);
    
    if (!scheduler_debugged_idle && next == idle)
    {
        scheduler_debugged_idle = 1;
        terminal_writeIn("=== IDLE SWITCH DEBUG ===");
        terminal_write("next process: ");

        if (next->process != 0)
        {
            terminal_write_hex(next->process->pid);
        }

        else
        {
            terminal_write("NULL");
        }

        terminal_write("\n");
        terminal_write("current process PID BEFORE set: ");

        if (process_current() != 0)
        {
            terminal_write_hex(process_current()->pid);
        }

        else
        {
            terminal_write("NULL");
        }

        terminal_write("\n");
        terminal_writeIn("Calling process_set_current(kernel)...");
        process_set_current(next->process);
        terminal_write("current process PID AFTER set: ");

        if (process_current() != 0)
        {
            terminal_write_hex(process_current()->pid);
        }

        else
        {
            terminal_write("NULL");
        }

        terminal_write("\n");
        terminal_writeIn("=== END DEBUG ===");
    }

    process_reap_terminated();
    return next->saved_esp;
}