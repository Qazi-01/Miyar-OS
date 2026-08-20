#include "process/scheduler.h"
#include "process/thread.h"
#include "arch/x86/context.h"

void scheduler_init(void)
{
}

uint32_t scheduler_schedule(struct registers *r)
{
    thread_t *current = thread_current();

    if (current == 0)
    {
        return (uint32_t)r;
    }

    thread_t *next = thread_dequeue();

    if (next == 0)
    {
        return (uint32_t)r;
    }

    current->state = THREAD_READY;
    thread_enqueue(current);
    next->state = THREAD_RUNNING;
    thread_set_current(next);
    current->saved_esp = (uint32_t)r;

    return next->saved_esp;
}