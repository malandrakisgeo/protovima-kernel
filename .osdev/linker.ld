OUTPUT_FORMAT("elf32-i386")
ENTRY(_start)

 /*MEMORY {
    bootsector(rwxai) : ORIGIN = 0x70000, LENGTH = 5k 0x7c00,  512 Default master boot record memory address on 8088 
}*/
INPUT(stage0.o kernel.o)
OUTPUT(os-img.bin)

SECTIONS
{
	. = 0x100000;
	.text : {*(.text)}
	.data : {*(.data)}
	.bss  : {*(.bss)}
}