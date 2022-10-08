

#define FRAME_SIZE 4096 //physical memory
#define PAGE_SIZE 4096 //virtual memory
#define PROCESS_MEMORY_START_ADDRESS 0x9000 //TODO: Create a memory map 


typedef struct phys_mem_frame{
    int *physical_address_start; 
    short int flags; //first bit: frame in use
}phys_mem_frame;

//total 48bits
typedef struct page{
    int *virtual_address_start; 
    short int flags; //first bit: page in use
}page;

typedef struct free_pages {
	struct page *prev;
	struct page *next;
}free_pages;




typedef struct page_table{
    int index;
    int *next_level_address; //leads to another page table (levels 1-3), or the the frame itself (level 4).
    int flags;
}page_table;

typedef struct e820_entry {
	unsigned int addr;
	unsigned int addr_high;
	unsigned int length;
	unsigned int length_high;
	unsigned int type;
	unsigned int pad;
}e820_entry;

typedef struct memory_entry {
	unsigned int start_addr;
	unsigned long length;
}memory_entry;

struct boot_param {
	struct e820_entry memMapp[256];
	unsigned long e820_num;
};

extern void boot_memory_init();


