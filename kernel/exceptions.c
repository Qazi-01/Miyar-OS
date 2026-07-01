#include "exceptions.h"
#include "panic.h"

void isr0_handler(void)
{
    panic("Divide by Zero Exception");
}