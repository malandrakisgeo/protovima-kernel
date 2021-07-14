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
#define E820_TYPE_FREE		1
#define PGSIZE	0x1000

 typedef struct memoryMapEntry{
    uint32 baseLow; 
    uint32 baseHigh;
    uint32 length;
    unsigned int length_high;   
    uint32 type;
    uint32 acpi_null; 
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


struct e820_entry {
	unsigned int addr;
	unsigned int addr_high;
	unsigned int length;
	unsigned int length_high;
	unsigned int type;
	unsigned int pad;
};

struct boot_paranm {
	struct e820_entry e820_list[128];
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
 
#ifndef COMMON_H
#define COMMON_H

// Some nice typedefs, to standardise sizes across platforms.
// These typedefs are written for 32-bit X86.
typedef unsigned int   u32int;
typedef          int   s32int;
typedef unsigned short u16int;
typedef          short s16int;
typedef unsigned char  u8int;
typedef          char  s8int;

void outb(u16int port, u8int value);
u8int inb(u16int port);
u16int inw(u16int port);

#endif
