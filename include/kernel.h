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
#define VGA_ORIGINAL 0xB8000
#define E820_ADDRESS 0x0A000 //0x82500   0xE100     0xFC04    0x16104 0x8c00
#define E820_ADDRESS2	((void *)VADDR(E820_ADDRESS))
#define E820_ADDRESS5 0x00000dfd0 
#define E820_ADDRESS4 0x1000
#define BUFSIZE 2200
#define KERNEL_BASE	0x10000


#define VADDR(pa)	((pa) + KERNEL_BASE)
#define NULL 0
#define E820_TYPE_FREE		1
#define PGSIZE	0x1000
#define VGA_ADDRESS2	((void *)VADDR(VGA_ORIGINAL))

typedef struct e820_entry {
	unsigned int addr;
	unsigned int addr_high;
	unsigned int length;
	unsigned int length_high;
	unsigned int type;
	unsigned int pad;
};

struct boot_param {
	struct e820_entry memMapp[128];
	unsigned long e820_num;
};


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

/*void outb(u16int port, u8int value);
u8int inb(u16int port);
u16int inw(u16int port);
*/
#endif
