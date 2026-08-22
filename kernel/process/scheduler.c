#include "process/process.h"
#include "process/scheduler.h"
#include "process/thread.h"
#include "memory/address_space.h"

void scheduler_init(void)
{
    thread_init();
}

uint32_t scheduler_schedule(struct registers *r)
{
    thread_t *current = thread_current();
    thread_t *idle = thread_idle();

    thread_reap_terminated(current);

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

    return next->saved_esp;
}