#include "exceptions.h"
#include "panic.h"

void isr0_handler(struct registers *regs)
{
    (void)regs;

    panic("Divide by Zero Exception");
}