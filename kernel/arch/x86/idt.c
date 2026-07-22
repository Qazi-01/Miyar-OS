#include <stdint.h>
#include "arch/x86/idt.h"

#define DECLARE_ISR(n) extern void isr_stub_##n(void)

DECLARE_ISR(0);
DECLARE_ISR(1);
DECLARE_ISR(2);
DECLARE_ISR(3);
DECLARE_ISR(4);
DECLARE_ISR(5);
DECLARE_ISR(6);
DECLARE_ISR(7);
DECLARE_ISR(8);
DECLARE_ISR(9);
DECLARE_ISR(10);
DECLARE_ISR(11);
DECLARE_ISR(12);
DECLARE_ISR(13);
DECLARE_ISR(14);
DECLARE_ISR(15);
DECLARE_ISR(16);
DECLARE_ISR(17);
DECLARE_ISR(18);
DECLARE_ISR(19);
DECLARE_ISR(20);
DECLARE_ISR(21);
DECLARE_ISR(22);
DECLARE_ISR(23);
DECLARE_ISR(24);
DECLARE_ISR(25);
DECLARE_ISR(26);
DECLARE_ISR(27);
DECLARE_ISR(28);
DECLARE_ISR(29);
DECLARE_ISR(30);
DECLARE_ISR(31);

#define DECLARE_IRQ(n) extern void irq_stub_##n(void)

DECLARE_IRQ(0);
DECLARE_IRQ(1);
DECLARE_IRQ(2);
DECLARE_IRQ(3);
DECLARE_IRQ(4);
DECLARE_IRQ(5);
DECLARE_IRQ(6);
DECLARE_IRQ(7);
DECLARE_IRQ(8);
DECLARE_IRQ(9);
DECLARE_IRQ(10);
DECLARE_IRQ(11);
DECLARE_IRQ(12);
DECLARE_IRQ(13);
DECLARE_IRQ(14);
DECLARE_IRQ(15);

struct idt_entry
{
    uint16_t offset_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t type_attr;
    uint16_t offset_high;
} __attribute__((packed));

struct idt_ptr
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

static struct idt_entry idt[256];

static void idt_set_gate(uint8_t num, uint32_t handler, uint16_t selector, uint8_t flags)
{
    idt[num].offset_low = handler & 0xFFFF;
    idt[num].selector = selector;
    idt[num].zero = 0;
    idt[num].type_attr = flags;
    idt[num].offset_high = (handler >> 16) & 0xFFFF;
}

static void idt_load(struct idt_ptr *idtp)
{
    __asm__ volatile("lidt (%0)" :: "r"(idtp));
}

void idt_init(void)
{
    struct idt_ptr idtp;

    idtp.limit = sizeof(idt) - 1;
    idtp.base = (uint32_t)idt;
    
    #define SET_ISR(n) idt_set_gate(n, (uint32_t)isr_stub_##n, 0x08, 0x8E)

    SET_ISR(0);
    SET_ISR(1);
    SET_ISR(2);
    SET_ISR(3);
    SET_ISR(4);
    SET_ISR(5);
    SET_ISR(6);
    SET_ISR(7);
    SET_ISR(8);
    SET_ISR(9);
    SET_ISR(10);
    SET_ISR(11);
    SET_ISR(12);
    SET_ISR(13);
    SET_ISR(14);
    SET_ISR(15);
    SET_ISR(16);
    SET_ISR(17);
    SET_ISR(18);
    SET_ISR(19);
    SET_ISR(20);
    SET_ISR(21);
    SET_ISR(22);
    SET_ISR(23);
    SET_ISR(24);
    SET_ISR(25);
    SET_ISR(26);
    SET_ISR(27);
    SET_ISR(28);
    SET_ISR(29);
    SET_ISR(30);
    SET_ISR(31);

    #define SET_IRQ(n) idt_set_gate(32 + n, (uint32_t)irq_stub_##n, 0x08, 0x8E)

    SET_IRQ(0);
    SET_IRQ(1);
    SET_IRQ(2);
    SET_IRQ(3);
    SET_IRQ(4);
    SET_IRQ(5);
    SET_IRQ(6);
    SET_IRQ(7);
    SET_IRQ(8);
    SET_IRQ(9);
    SET_IRQ(10);
    SET_IRQ(11);
    SET_IRQ(12);
    SET_IRQ(13);
    SET_IRQ(14);
    SET_IRQ(15);

    idt_load(&idtp);
}
