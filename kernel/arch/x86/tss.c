#include <stdint.h>
#include "arch/x86/tss.h"


#define GDT_KERNEL_DATA 0x10

extern void tss_flush(void);
tss_entry_t tss;

void tss_init(void)
{
    tss = (tss_entry_t){0};
    tss.ss0 = GDT_KERNEL_DATA;

    static uint8_t temporary_kernel_stack[4096];
    __attribute__((alligned(16)));
    tss.esp0 = (uint32_t)(temporary_kernel_stack + sizeof(temporary_kernel_stack));
    tss.iomap_base = sizeof(tss_entry_t);
    tss_flush();
}