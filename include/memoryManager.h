#pragma once



#define FRAME_SIZE 4096 //physical memory, bytes
#define PAGE_SIZE 4096 //virtual memory, bytes
#define FREE_PAGE_ENTRIES 1024
#define CONT_MEM_STRUCT_SIZE 64 //a contiguous_mem_struct (nee free_page) structure consists of two long ints, or 64 bits

#define FREE_PAGE_TABLE_SIZE FREE_PAGE_ENTRIES*CONT_MEM_STRUCT_SIZE //bits

#define PROCESS_MEMORY_START_ADDRESS 0xC000 //TODO: Create a memory map 

#ifndef MEMORY_STRUCTS
#define MEMORY_STRUCTS


typedef struct phys_mem_frame{
    int *physical_address_start; 
    short int flags; //first bit: frame in use
}phys_mem_frame;

//total 48bits
typedef struct page{
    long int *start_address; //32bits
    int flags; //16bits
}page;

typedef struct contiguous_mem_struct {
	struct page *unused_page; //i.e. long int
	struct contiguous_mem_struct *next; //i.e. long int
}contiguous_mem_struct;



typedef struct page_table{ //FOR FUTURE USE
    int index;
    int *next_level_address; //leads to another page table (levels 1-3), or the the frame itself (level 4).
    int flags;
}page_table;

typedef struct e820_entry {
	unsigned int addr;
	unsigned int addr_high;
	unsigned int length; //in bytes
	unsigned int length_high;
	unsigned int type;
	unsigned int pad;
}e820_entry;

typedef struct memory_entry {
	unsigned long start_addr;
	unsigned long length;
	short reserved;
}memory_entry;

typedef struct contiguous_mem_entries{
	struct memory_entry entries[256]; //definitely no more than the e820 entries
} contiguous_mem_entries;

typedef struct boot_param {
	struct e820_entry memMapp[256];
	unsigned long e820_num;
};

#endif

void boot_memory_init();
void *page_alloc();
volatile void *malloc(long size_in_bytes, int called_by_user);
void initialize_paging();
void *page_memory_alloc();
