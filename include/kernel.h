#pragma once

#ifndef KERNEL_H
#define KERNEL_H

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long ulong32;
typedef unsigned long long ulong64;

#define MaxYInitial 25
#define MaxXInitial 80
#define KERNEL_BASE	0x7000


#define VGA_ADDRESS 0xB8000 //Default for colour monitors. TODO: Check if it actually is just for the Real mode, and the protected has some more effective way than VGA access
#define E820_ADDRESS 0x0B00 
#define BUFSIZE 2200


#define VADDR(pa)	((pa) + KERNEL_BASE)
#define NULL 0

#define E820_TYPE_FREE		1 //https://uefi.org/sites/default/files/resources/ACPI_4_Errata_A.pdf, pg. 477
#define E820_TYPE_RESERVED  2
#define E820_TYPE_ACPI_RECLAIM  3
#define E820_TYPE_NVS  4
#define E820_TYPE_UNUSUABLE 5
#define E820_TYPE_DISABLED  6


#define PGSIZE	0x1000
#define VGA_ADDRESS2	((void *)VADDR(VGA_ORIGINAL))


#define current_page_table_size 1024

#define max_procs 8








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

#endif
