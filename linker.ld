ENTRY(_start)

SECTIONS
{
  . = 1M;

  .multiboot ALIGN(4) : AT(0)
  {
    KEEP(*(.multiboot))
  }

  . = ALIGN(4K);
  
  .text : AT(ADDR(.text) - 1M)
  {
    *(.text)
  }

  .data : AT(ADDR(.data) - 1M)
  {
    *(.data)
  }

  .bss : AT(ADDR(.bss) - 1M)
  {
    *(COMMON)
    *(.bss)
  }

  _kernel_end = .;
}