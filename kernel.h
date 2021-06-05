#ifndef KERNEL_H
#define KERNEL_H

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long ulong32;
typedef unsigned long long ulong64;

#define MaxYInitial 25
#define MaxXInitial 80

#define VGA_ADDRESS 0xB8000
#define E820_ADDRESS 0x8c00 //0x82500   0xE100     0xFC04    0x16104 0x8c00
#define BUFSIZE 2200
#define KERNEL_BASE	0x1000

#define VADDR(pa)	((pa) + KERNEL_BASE*16)
#define VADDR2(pa)	((pa) + KERNEL_BASE)
#define NULL 0


 typedef struct memoryMapEntry{
    ulong64 baseLow; //unsigned long long equivalent
    ulong64 baseHigh;
    ulong32 length;   
    ulong32 type;
    ulong32 acpi_null; 
    } memoryMapEntry; 

/*typedef struct memoryMapEntry{
    unsigned int baseLow; //unsigned long long equivalent
    unsigned int baseHigh;
    unsigned int length;   
    unsigned int type;
    unsigned int acpi_null; 
    } memoryMapEntry;  */

struct boot_param {
	struct memoryMapEntry memMapp[128];
	unsigned int e820_num;
};

uint16* vga_buffer;


enum vga_color {
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    GREY,
    DARK_GREY,
    BRIGHT_BLUE,
    BRIGHT_GREEN,
    BRIGHT_CYAN,
    BRIGHT_RED,
    BRIGHT_MAGENTA,
    YELLOW,
    WHITE,
};

#endif
 
