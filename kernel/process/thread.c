#include "process/thread.h"
#include "memory/heap.h"
#include "terminal.h"
#include <stdint.h>

static uint32_t next_tid = 1;
static thread_t *current_thread = 0;
static thread_t *ready_queue_head = 0;
static thread_t *ready_queue_tail = 0;
static thread_t *idle_thread_instance = 0;
static thread_t *blocking_test_target = 0;

static void first_thread(void);
static void second_thread(void);
static void idle_thread(void);
static void blocking_thread(void);

void thread_init(void)
{
    next_tid = 1;
    current_thread = 0;
    ready_queue_head = 0;
    ready_queue_tail = 0;
    
    idle_thread_instance = thread_create(idle_thread, "idle");
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

    thread->frame->gs = 0x10;
    thread->frame->fs = 0x10;
    thread->frame->es = 0x10;
    thread->frame->ds = 0x10;
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

void thread_test_start(void)
{
    thread_t *thread1 = thread_create(first_thread, "First");

    if (thread1 == 0)
    {
        return;
    }

    thread_t *thread2 = thread_create(second_thread, "Second");

    if (thread2 == 0)
    {
        thread_destroy(thread1);
        return;
    }

    thread_enqueue(thread1);
    thread_enqueue(thread2);

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

    if (thread == current_thread)
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
    thread_t *idle = thread_idle();

    if (current == 0 || current == idle)
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

static void first_thread(void)
{
    while (1)
    {
        terminal_writeIn("Thread 1 is running.");
    }
}

static void second_thread(void)
{
    while (1)
    {
        terminal_writeIn("Thread 2 is running.");
    }
}

void thread_block(void)
{
    thread_t *current = thread_current();

    if (current == 0)
    {
        return;
    }

    current->state = THREAD_BLOCKED;
    thread_t *next = thread_dequeue();

    if (next == 0)
    {
        next = thread_idle();

        if (next == 0)
        {
            return;
        }
    }

    next->state = THREAD_RUNNING;
    thread_set_current(next);
    x86_context_switch(&current->saved_esp, next->saved_esp);
}

void thread_unblock(thread_t *thread)
{
    if (thread == 0)
    {
        return;
    }

    if (thread->state != THREAD_BLOCKED)
    {
        return;
    }

    thread_enqueue(thread);
}

void thread_terminate(void)
{
    thread_t *current = thread_current();

    if (current == 0)
    {
        return;
    }

    current->state = THREAD_TERMINATED;
}

static void idle_thread(void)
{
    while (1)
    {
        __asm__ volatile("hlt");
    }
}

thread_t *thread_idle(void)
{
    return idle_thread_instance;
}

static void blocking_thread(void)
{
    terminal_writeIn("Blocking thread started.");
    thread_block();
    terminal_writeIn("Blocking thread resumed.");

    while (1)
    {
        thread_yield();
    }
}

static void blocking_test_controller(void)
{
    terminal_writeIn("Controller thread started.");

    while (blocking_test_target == 0)
    {
        thread_yield();
    }

    while (blocking_test_target->state != THREAD_BLOCKED)
    {
        thread_yield();
    }

    terminal_writeIn("Unblocking thread.");
    thread_unblock(blocking_test_target);
    blocking_test_target = 0;

    while (1)
    {
        thread_yield();
    }
}

void thread_blocking_test_start(void)
{
    thread_t *blocked = thread_create(blocking_thread, "Blocked");
    thread_t *controller = thread_create(blocking_test_controller, "Controller");

    if (blocked == 0 || controller == 0)
    {
        if (blocked != 0)
        {
            thread_destroy(blocked);
        }

        if (controller != 0)
        {
            thread_destroy(controller);
        }

        return;
    }

    blocking_test_target = blocked;
    thread_enqueue(blocked);
    thread_enqueue(controller);
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