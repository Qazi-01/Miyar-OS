#include "process/thread.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static uint32_t next_tid = 1;

void thread_init(void)
{
    next_tid = 1;
}

thread_t *thread_create(void (*entry)(void), const char *name)
{
    (void)entry;
    (void)name;
    
    return 0;
}

void thread_destroy(thread_t *thread)
{
    (void)thread;
}