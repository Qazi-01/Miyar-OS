#ifndef THREAD_H
#define THREAD_H

#include <stdint.h>
#include <stdbool.h>
#include <arch/x86/context.h>

#define THREAD_NAME_MAX 32
#define THREAD_STACK_SIZE 4096

typedef enum {
    THREAD_READY,
    THREAD_RUNNING,
    THREAD_BLOCKED,
    THREAD_TERMINATED
} thread_state_t;

typedef struct thread {
    uint32_t tid;
    thread_state_t state;
    uint32_t kernel_stack;
    uint32_t kernel_stack_top;
    uint32_t saved_esp;
    x86_thread_frame_t *frame;
    void (*entry)(void);
    char name[THREAD_NAME_MAX];
    struct thread *next;
} thread_t;

void thread_init(void);
thread_t *thread_create(void (*entry)(void), const char *name);
void thread_destroy(thread_t *thread);
void thread_test_start(void);
void thread_set_current(thread_t *thread);
thread_t *thread_current(void);
void thread_enqueue(thread_t *thread);
thread_t *thread_dequeue(void);
void thread_yield(void);
void thread_block(void);
void thread_unblock(thread_t *thread);
void thread_terminate(void);
thread_t *thread_idle(void);
void thread_blocking_test_start(void);

#endif