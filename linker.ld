ENTRY(_start)

SECTIONS
{
    .multiboot 0x100000 : AT(ADDR(.multiboot) & 0xFFFF)
    {
        KEEP(*(.multiboot))
    }

    . = 0x100000 + SIZEOF(.multiboot);

    .text ALIGN(4K) : AT(ADDR(.text))
    {
        *(.text)
    }

    .data ALIGN(4K) : AT(ADDR(.data))
    {
        *(.data)
    }

    .bss ALIGN(4K) : AT(ADDR(.bss))
    {
        *(COMMON)
        *(.bss)
    }

    _kernel_end = .;
}