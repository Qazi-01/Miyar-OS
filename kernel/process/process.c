#include "process/process.h"
#include "process/thread.h"
#include "memory/heap.h"
#include "memory/address_space.h"

static uint32_t next_pid = 1;
static process_t *current_process = 0;
static process_t *kernel_process = 0;
static process_t *process_list_head = 0;
static process_t *process_list_tail = 0;

void process_init(void)
{
    next_pid = 1;
    current_process = 0;
    kernel_process = process_create("kernel");

    if (kernel_process != 0)
    {
        current_process = kernel_process;
    }
}

process_t *process_create(const char *name)
{
    process_t *process = (process_t *)kmalloc(sizeof(process_t));

    if (process == 0)
    {
        return 0;
    }

    process->address_space = address_space_create();

    if (process->address_space == 0)
    {
        kfree(process);
        return 0;
    }

    process->pid = next_pid++;
    process->state = PROCESS_RUNNING;
    process->address_space = 0;
    process->parent = 0;
    process->thread_count = 0;
    process->next = 0;

    for (uint32_t i = 0; i < PROCESS_NAME_MAX; i++)
    {
        process->name[i] = name[i];

        if (name != 0 && name[i] != 0)
        {
            process->name[i] = name[i];    
        }

        else
        {
            break;
        }
    }

    if (process_list_tail == 0)
    {
        process_list_head = process;
        process_list_tail = process;
    }

    else
    {
        process_list_tail->next = process;
        process_list_tail = process;
    }

    return process;
}

void process_destroy(process_t *process)
{
    if (process == 0)
    {
        return;
    }

    if (process->address_space != 0)
    {
        address_space_destroy(process->address_space);
        process->address_space = 0;
    }

    if (process == kernel_process)
    {
        return;
    }

    if (process->thread_count != 0)
    {
        return;
    }

    if (current_process == process)
    {
        current_process = 0;
    }

    process_t *previous = 0;
    process_t *current = process_list_head;

    while (current != 0)
    {
        if (current == process)
        {
            if (process == 0)
            {
                process_list_head = current->next;
            }

            else
            {
                previous->next = current->next;
            }

            if (process_list_tail == current)
            {
                process_list_tail = previous;
            }

            break;
        }

        previous = current;
        current = current->next;
    }

    kfree(process);
}

process_t *process_get_current(void)
{
    return current_process;
}

process_t *process_find(uint32_t pid)
{
    process_t *process = process_list_head;

    while (process != 0)
    {
        if (process->pid == pid)
        {
            return process;
        }

        process = process->next;
    }

    return 0;
}

void process_exit(process_t *process)
{
    if (process == 0)
    {
        return;
    }

    if (process == kernel_process)
    {
        return;
    }

    process->state = PROCESS_TERMINATED;
    thread_t *current = thread_current();

    if (current != 0 && current->process == process)
    {
        thread_terminate();
    }
}