#include "process/thread.h"
#include "process/process.h"
#include "memory/heap.h"
#include "terminal.h"
#include <stdint.h>

static uint32_t next_tid = 1;
static thread_t *current_thread = 0;
static thread_t *ready_queue_head = 0;
static thread_t *ready_queue_tail = 0;
static thread_t *idle_thread_instance = 0;
static thread_t *blocking_test_target = 0;
static thread_t bootstrap_thread;
static thread_t *terminated_queue_head = 0;
static thread_t *terminated_queue_tail = 0;
static volatile uint32_t blocking_test_resumed = 0;
static volatile uint32_t blocking_test_paused = 0;
static volatile uint32_t blocking_test_complete = 0;
static volatile uint32_t blocking_test_passed = 0;

static void first_thread(void);
static void second_thread(void);
static void idle_thread(void);
static void blocking_thread(void);

void thread_init(void)
{
    terminal_writeIn("Thread_init: Started");

    next_tid = 1;
    current_thread = 0;
    ready_queue_head = 0;
    ready_queue_tail = 0;
    terminated_queue_head = 0;
    terminated_queue_tail = 0;

    terminal_writeIn("Thread_init: checking process");

    process_t *kernel_process = process_current();

    if (kernel_process == 0)
    {
        terminal_writeIn("Thread_init: process is NULL");
        return;
    }

    terminal_writeIn("Thread_init: process OK");

    bootstrap_thread.tid = next_tid++;
    bootstrap_thread.state = THREAD_RUNNING;
    bootstrap_thread.process = kernel_process;
    bootstrap_thread.kernel_stack = 0;
    bootstrap_thread.kernel_stack_top = 0;
    bootstrap_thread.saved_esp = 0;
    bootstrap_thread.frame = 0;
    bootstrap_thread.entry = 0;
    bootstrap_thread.next = 0;

    bootstrap_thread.name[0] = 'k';
    bootstrap_thread.name[1] = 'e';
    bootstrap_thread.name[2] = 'r';
    bootstrap_thread.name[3] = 'n';
    bootstrap_thread.name[4] = 'e';
    bootstrap_thread.name[5] = 'l';
    bootstrap_thread.name[6] = 0;

    kernel_process->thread_count++;
    current_thread = &bootstrap_thread;

    terminal_writeIn("Thread_init: bootstrap OK");

    idle_thread_instance = thread_create(idle_thread, "idle");

    terminal_writeIn("Thread_init: idle create returned");

    if (idle_thread_instance == 0)
    {
        terminal_writeIn("Thread_init: idle FAILED");
        return;
    }

    terminal_writeIn("Thread_init: idle OK");
}

thread_t *thread_create_in_process(process_t *process,void (*entry)(void), const char *name)
{
    if (entry == 0 || process == 0)
    {
        return 0;
    }

    if (process->state != PROCESS_RUNNING)
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
    thread->process = process;
    process->thread_count++;
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

thread_t *thread_create(void (*entry)(void), const char *name)
{
    return thread_create_in_process(process_current(), entry, name);
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

    if (thread->process != 0 && thread->process->thread_count > 0)
    {
        thread->process->thread_count--;
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

    __asm__ volatile("cli");
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

    __asm__ volatile("cli");
    current->state = THREAD_BLOCKED;
    thread_t *next = thread_dequeue();

    if (next == 0)
    {
        next = thread_idle();

        if (next == 0)
        {
            __asm__ volatile("sti");
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

    __asm__ volatile("cli");
    thread_enqueue(thread);
    __asm__ volatile("sti");
}

void thread_reap_terminated(thread_t *current)
{
    thread_t *previous = 0;
    thread_t *thread = terminated_queue_head;

    while (thread != 0)
    {
        thread_t *next = thread->next;

        if (thread != current)
        {
            if (previous == 0)
            {
                terminated_queue_head = next;
            }

            else
            {
                previous->next = next;
            }

            if (terminated_queue_tail == thread)
            {
                terminated_queue_tail = previous;
            }

            thread->next = 0;

            if (thread->process != 0 && thread->process->thread_count > 0)
            {
                thread->process->thread_count--;
            }

            if (thread->kernel_stack != 0)
            {
                terminal_writeIn("Thread reaper: reaping terminated thread.");
                kfree((void *)thread->kernel_stack);
                terminal_writeIn("Thread reaper: terminated thread freed.");
            }

            kfree(thread);
        }

        else
        {
            previous = thread;
        }

        thread = next;
    }
}

void thread_terminate(void)
{
    thread_t *current = thread_current();

    if (current == 0)
    {
        return;
    }

    current->state = THREAD_TERMINATED;
    current->next = 0;

    if (terminated_queue_tail == 0)
    {
        terminated_queue_head = current;
        terminated_queue_tail = current;
    }

    else
    {
        terminated_queue_tail->next = current;
        terminated_queue_tail = current;
    }
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

    blocking_test_resumed = 1;
    thread_terminate();

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
    blocking_test_paused = 1;
    blocking_test_target = 0;

    while (!blocking_test_resumed)
    {
        thread_yield();
    }

    thread_terminate();

    while (1)
    {
        thread_yield();
    }
}

void thread_blocking_test_start(void)
{
    blocking_test_target = 0;
    blocking_test_resumed  = 0;
    blocking_test_paused = 0;
    blocking_test_complete = 0;
    blocking_test_passed = 0;

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

        terminal_writeIn("Blocking test: thread creation failed.");
        return;
    }

    blocking_test_target = blocked;
    thread_enqueue(blocked);
    thread_enqueue(controller);
    terminal_writeIn("Blocking test threads queued.");
}