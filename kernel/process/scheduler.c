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

    if (current == 0)
    {
        return (uint32_t)r;
    }

    thread_t *next = thread_dequeue();

    if (next == 0)
    {
        next = idle;

        if (next == 0)
        {
            return (uint32_t)r;
        }
    }

    if (current != idle &&current->state != THREAD_TERMINATED)
    {
        current->state = THREAD_READY;
        thread_enqueue(current);
    }
    
    next->state = THREAD_RUNNING;
    thread_set_current(next);

    if (next->process != 0 &&current->process != next->process)
    {
        address_space_activate(next->process->address_space);
    }

    current->saved_esp = (uint32_t)r;

    return next->saved_esp;
}