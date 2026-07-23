#include "drivers/keyboard.h"

#define KEYBOARD_BUFFER_SIZE 128

static char keyboard_buffer[KEYBOARD_BUFFER_SIZE];

static volatile unsigned int buffer_head = 0;
static volatile unsigned int buffer_tail = 0;

void keyboard_init(void)
{
    buffer_head = 0;
    buffer_tail = 0;
}

void keyboard_buffer_put(char c)
{
    unsigned int next = (buffer_head + 1) % KEYBOARD_BUFFER_SIZE;

    /* Buffer full */
    if (next == buffer_tail)
        return;

    keyboard_buffer[buffer_head] = c;
    buffer_head = next;
}

char keyboard_getchar(void)
{
    /* Disable interrupts to prevent race condition */
    __asm__ volatile("cli");
    
    if (buffer_head == buffer_tail) {
        __asm__ volatile("sti");
        return 0;
    }

    char c = keyboard_buffer[buffer_tail];
    buffer_tail = (buffer_tail + 1) % KEYBOARD_BUFFER_SIZE;

    __asm__ volatile("sti");
    return c;
}
