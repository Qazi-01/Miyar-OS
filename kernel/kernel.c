#include <stdint.h>

__attribute__((used))
void kernel_main(uint32_t magic, uint32_t* info) {
    (void)magic;
    (void)info;

    /* Force a character out of the serial port without waiting */
    __asm__ volatile ("outb %0, %1" : : "a"((uint8_t)'X'), "Nd"((uint16_t)0x3F8));

    for (;;) {
        __asm__ volatile ("hlt");
    }
}
