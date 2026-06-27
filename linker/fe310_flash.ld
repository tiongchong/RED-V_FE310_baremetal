OUTPUT_ARCH(riscv)
ENTRY(_start)

MEMORY
{
    flash (rxai) : ORIGIN = 0x20010000, LENGTH = 0x003F0000
    ram   (wxa)  : ORIGIN = 0x80000000, LENGTH = 16K
}

REGION_ALIAS("REGION_TEXT", flash);
REGION_ALIAS("REGION_RODATA", flash);
REGION_ALIAS("REGION_DATA", ram);
REGION_ALIAS("REGION_BSS", ram);

_stack_size = 2K;

SECTIONS
{
    .text :
    {
        KEEP(*(.text.start))
        KEEP(*(.init))
        *(.text .text.*)
        *(.gnu.linkonce.t.*)
    } > REGION_TEXT

    .rodata :
    {
        *(.rodata .rodata.*)
        *(.srodata .srodata.*)
        *(.gnu.linkonce.r.*)
    } > REGION_RODATA

    .preinit_array :
    {
        PROVIDE_HIDDEN(__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN(__preinit_array_end = .);
    } > REGION_RODATA

    .init_array :
    {
        PROVIDE_HIDDEN(__init_array_start = .);
        KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
        KEEP(*(.init_array EXCLUDE_FILE(*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o) .ctors))
        PROVIDE_HIDDEN(__init_array_end = .);
    } > REGION_RODATA

    .fini_array :
    {
        PROVIDE_HIDDEN(__fini_array_start = .);
        KEEP(*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
        KEEP(*(.fini_array EXCLUDE_FILE(*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o) .dtors))
        PROVIDE_HIDDEN(__fini_array_end = .);
    } > REGION_RODATA

    .data :
    {
        . = ALIGN(4);
        _sdata = .;
        PROVIDE(__global_pointer$ = . + 0x800);
        *(.data .data.*)
        *(.sdata .sdata.*)
        *(.gnu.linkonce.d.*)
        . = ALIGN(4);
        _edata = .;
    } > REGION_DATA AT > REGION_TEXT

    _sidata = LOADADDR(.data);

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
    } > REGION_BSS

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
