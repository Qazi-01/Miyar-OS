#include "process/thread.h"
#include "memory/heap.h"
#include "terminal.h"
#include <stdint.h>

static uint32_t next_tid = 1;
static thread_t *current_thread = 0;
static thread_t *ready_queue_head = 0;
static thread_t *ready_queue_tail = 0;

void thread_init(void)
{
    next_tid = 1;
    current_thread = 0;
    ready_queue_head = 0;
    ready_queue_tail = 0;
}

thread_t *thread_create(void (*entry)(void), const char *name)
{
    if (entry == 0)
    {
        return 0;
    }

    thread_t *thread = (thread_t *)kmalloc(sizeof(thread_t));

    if (thread == 0)
    {
        return 0;
    }

    uint8_t *stack = (uint8_t *)kmalloc(THREAD_STACK_SIZE);

    if (stack == 0)
    {
        kfree(thread);
        return 0;
    }

    thread->tid = next_tid++;
    thread->state = THREAD_READY;
    thread->kernel_stack = (uint32_t)stack;
    thread->kernel_stack_top = (uint32_t)(stack + THREAD_STACK_SIZE);
    thread->entry = entry;
    thread->next = 0;

    for (uint32_t i = 0; i < THREAD_NAME_MAX; i++)
    {
        thread->name[i] = 0;
        
        if (name != 0 && name[i] != 0)
        {
            thread->name[i] = name[i];
        }
        else
        {
            break;
        }
    }
    
    uint32_t frame_address = thread->kernel_stack_top - sizeof(x86_thread_frame_t);
    thread->frame = (x86_thread_frame_t *)frame_address;

    thread->frame->gs = 0;
    thread->frame->fs = 0;
    thread->frame->es = 0;
    thread->frame->ds = 0;
    thread->frame->edi = 0;
    thread->frame->esi = 0;
    thread->frame->ebp = 0;
    thread->frame->esp = thread->kernel_stack_top;
    thread->frame->ebx = 0;
    thread->frame->edx = 0;
    thread->frame->ecx = 0;
    thread->frame->eax = 0;
    thread->frame->int_no = 0;
    thread->frame->err_code = 0;
    thread->frame->eip = (uint32_t)entry;
    thread->frame->cs = 0x08;
    thread->frame->eflags = 0x202;
    thread->saved_esp = frame_address;

    return thread;
}

static void thread_test(void)
{
    terminal_writeIn("Kernel thread started succesfully.");

    while (1)
    {
        thread_yield();
    }
}

void thread_test_start(void)
{
    thread_t *thread_a = thread_create(thread_test, "thread_a");

    if (thread_a == 0)
    {
        return;
    }

    thread_t *thread_b = thread_create(thread_test, "thread_b");

    if (thread_b == 0)
    {
        return;
    }

    thread_enqueue(thread_a);
    thread_enqueue(thread_b);

    thread_t *first = thread_dequeue();

    if (first == 0)
    {
        return;
    }

    thread_set_current(first);
    first->state = THREAD_RUNNING;
    uint32_t old_esp = 0;
    x86_context_switch(&old_esp, first->saved_esp);

    while (1)
    {
        __asm__ volatile("hlt");
    }
}

void thread_destroy(thread_t *thread)
{
    if (thread == 0)
    {
        return;
    }

    if (thread->kernel_stack != 0)
    {
        kfree((void *)thread->kernel_stack);
    }

    kfree(thread);
}

void thread_set_current(thread_t *thread)
{
    current_thread = thread;
}

thread_t *thread_current(void)
{
    return current_thread;
}

void thread_enqueue(thread_t *thread)
{
    if (thread == 0)
    {
        return;
    }

    thread->next = 0;
    thread->state = THREAD_READY;

    if (ready_queue_tail == 0)
    {
        ready_queue_head = thread;
        ready_queue_tail = thread;
        return;
    }

    else
    {
        ready_queue_tail->next = thread;
        ready_queue_tail = thread;
    }
}

thread_t *thread_dequeue(void)
{
    thread_t *thread = ready_queue_head;

    if (thread == 0)
    {
        return 0;
    }

    ready_queue_head = thread->next;

    if (ready_queue_head == 0)
    {
        ready_queue_tail = 0;
    }

    thread->next = 0;
    return thread;
}

void thread_yield(void)
{
    thread_t *current = thread_current();

    if (current == 0)
    {
        return;
    }

    thread_t *next = thread_dequeue();

    if (next == 0)
    {
        return;
    }

    current->state = THREAD_READY;
    thread_enqueue(current);
    next->state = THREAD_RUNNING;
    thread_set_current(next);
    x86_context_switch(&current->saved_esp, next->saved_esp);
}