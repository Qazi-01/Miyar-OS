#include <stdint.h>

#include "serial.h"
#include "terminal.h"
#include "vga.h"

__attribute__((used))
void kernel_main(uint32_t magic, uint32_t *multiboot_info) {
    (void)magic;
    (void)multiboot_info;

    serial_init();

    terminal_init();

    terminal_write("MiyarOS\n");

    kputs("Kernel booted.\n");
    kputs("MiyarOS (VGA written)\n");

    while (1) {
        __asm__ volatile ("hlt");
    }
}