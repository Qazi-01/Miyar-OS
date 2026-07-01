#include "exceptions.h"
#include "panic.h"

void exception_handler(void)
{
    panic("CPU Exception");
}