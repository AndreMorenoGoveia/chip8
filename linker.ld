SECTIONS
{
    . = 0x8000;          /* endereço onde o Pi coloca o kernel */
    .text : { *(.text*) }

    .rodata : { *(.rodata*) }

    .bss : {
        __bss_start = .;
        *(.bss*)
        *(COMMON)
        __bss_end = .;
    }

}
