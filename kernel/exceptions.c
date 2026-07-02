#include "exceptions.h"
#include "panic.h"

void exception_handler(struct registers *r)
{
    (void)r;
    panic("CPU Exception");
}
