OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(main)

SECTIONS
{
    . = 0x9000;
  .text : { *(.text) }
  .DATA : { [A-Z]*(.data) }
  .data : { *(.data) }
  .bss : { *(.bss) }

}