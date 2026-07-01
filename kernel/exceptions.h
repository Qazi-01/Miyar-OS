#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdint.h>

struct registers
{
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
};

void exception_handler(void);

#endif