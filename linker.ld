ENTRY(_start)

SECTIONS
{
  . = 1M;

  _kernel_start = .;
  
  .text ALIGN(4K) :
  {
    KEEP(*(.multiboot))
    *(.text*)
    *(.rodata*)
  }

  .data ALIGN(4K) :
  {
    *(.data*)
  }

  .bss ALIGN(4K) :
  {
    *(COMMON)
    *(.bss*)
  }

  _kernel_end = .;
}