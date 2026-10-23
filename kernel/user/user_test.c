#include <stdint.h>

void user_test(void)
{
    volatile uint32_t value = 0x12345678;
    (void)value;

    for (;;)
    {
        __asm__ volatile("hlt");
    }
}