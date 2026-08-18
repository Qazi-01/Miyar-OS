#ifndef THREAD_H
#define THREAD_H

#include <stdint.h>
#include <stdbool.h>

#define THREAD_NAME_MAX 32

typedef enum
{
    THREAD_READY,
    THREAD_RUNNING,
    THREAD_BLOCKED,
    THREAD_TERMINATED
} thread_state_t;

typedef struct thread
{
    uint32_t tid;
    thread_state_t state;
    uint32_t kernel_stack;
    uint32_t kernel_stack_top;
    void (*entry)(void);
    char name[THREAD_NAME_MAX];
    struct thread *next;
} thread_t;

void thread_init(void);
thread_t *thread_create(void (*entry)(void), const char *name);
void thread_destroy(thread_t *thread);

#endif