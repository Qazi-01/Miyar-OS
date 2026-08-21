#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include <stdbool.h>
#include "memory/address_space.h"

#define PROCESS_NAME_MAX 32

typedef struct process process_t;

typedef enum {
    PROCESS_RUNNING,
    PROCESS_TERMINATED
} process_state_t;

struct process {
    uint32_t pid;
    char name[PROCESS_NAME_MAX];
    address_space_t *address_space;
    process_t *parent;
    uint32_t thread_count;
    process_t *next;
    process_state_t state;
};

void process_init(void);
process_t *process_create(const char *name);
void process_destroy(process_t *process);
process_t *process_current(void);
process_t *process_find(uint32_t pid);
void process_exit(process_t *process);

#endif