#include "process/thread.h"
#include "memory/heap.h"
#include "terminal.h"
#include <stdint.h>

static uint32_t next_tid = 1;
static thread_t *current_thread = 0;

void thread_init(void)
{
    next_tid = 1;
    current_thread = 0;
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
        __asm__ volatile("hlt");
    }
}

void thread_test_start(void)
{
    thread_t *thread = thread_create(thread_test, "test");

    if (thread == 0)
    {
        return;
    }

    thread_set_current(thread);
    thread->state = THREAD_RUNNING;
    
    x86_context_restore(thread->saved_esp);

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