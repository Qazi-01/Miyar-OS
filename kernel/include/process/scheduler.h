#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include "arch/x86/exceptions.h"

void scheduler_init(void);
uint32_t scheduler_schedule(struct registers *r);

#endif