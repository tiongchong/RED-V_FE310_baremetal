OUTPUT_ARCH(riscv)
ENTRY(_start)

MEMORY
{
    ram (wxa) : ORIGIN = 0x80000000, LENGTH = 16K
}

_stack_size = 2K;

SECTIONS
{
    .text :
    {
        KEEP(*(.text.start))
        KEEP(*(.init))
        *(.text .text.*)
        *(.rodata .rodata.*)
        *(.srodata .srodata.*)
    } > ram

    .data :
    {
        . = ALIGN(4);
        _sidata = .;
        _sdata = .;
        PROVIDE(__global_pointer$ = . + 0x800);
        *(.data .data.*)
        *(.sdata .sdata.*)
        . = ALIGN(4);
        _edata = .;
    } > ram

    .bss (NOLOAD) :
    {
        . = ALIGN(4);
        _sbss = .;
        *(.scommon)
        *(.bss .bss.*)
        *(.sbss .sbss.*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } > ram

    .stack (NOLOAD) :
    {
        . = ALIGN(16);
        _stack_bottom = .;
        . += _stack_size;
        . = ALIGN(16);
        _stack_top = .;
    } > ram

    _end = .;
    PROVIDE(end = .);
}
