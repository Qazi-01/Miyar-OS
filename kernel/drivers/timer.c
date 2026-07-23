#include <stdint.h>

#include "drivers/io.h"
#include "drivers/timer.h"

#define PIT_FREQUENCY 100
#define PIT_COMMAND_PORT 0x43
#define PIT_CHANNEL0_PORT 0x40

static volatile uint32_t ticks = 0;

void timer_init(void)
{
    uint16_t divisor = (uint16_t)(1193182 / PIT_FREQUENCY);

    outb(PIT_COMMAND_PORT, 0x36);
    outb(PIT_CHANNEL0_PORT, (uint8_t)(divisor & 0xFF));
    outb(PIT_CHANNEL0_PORT, (uint8_t)((divisor >> 8) & 0xFF));
}

void timer_tick(void)
{
    ticks++;
}

uint32_t timer_ticks(void)
{
    /* Disable interrupts to safely read ticks */
    __asm__ volatile("cli");
    uint32_t result = ticks;
    __asm__ volatile("sti");
    return result;
}