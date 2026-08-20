#ifndef IRQ_H
#define IRQ_H

#include <stdint.h>
#include "exceptions.h"

uint32_t irq_handler(struct registers *r);
void timer_tick(void);

#endif