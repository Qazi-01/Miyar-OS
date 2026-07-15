#ifndef PANIC_H
#define PANIC_H

#include "exceptions.h"

void kernel_panic(const char *message, struct registers *r);

#endif