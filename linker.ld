OUTPUT_FORMAT(elf32-i386)
ENTRY(main)

SECTIONS
{
    . = 0x1000;
    .text : { *(.text.start) *(.text) }
    .data : { *(.data) }
    .bss  : { *(.bss) *(COMMON) }
}