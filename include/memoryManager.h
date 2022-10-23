

#define FRAME_SIZE 4096 //physical memory
#define PAGE_SIZE 4096 //virtual memory
#define FREE_PAGE_ENTRIES 1024
#define FREE_PAGE_STRUCTURE_SIZE 64 //a contiguous_mem_struct (nee free_page) structure consists of two long ints

#define FREE_PAGE_TABLE_SIZE FREE_PAGE_ENTRIES*FREE_PAGE_STRUCTURE_SIZE //virtual memory

#define PROCESS_MEMORY_START_ADDRESS 0x9000 //TODO: Create a memory map 


typedef struct phys_mem_frame{
    int *physical_address_start; 
    short int flags; //first bit: frame in use
}phys_mem_frame;

//total 48bits
typedef struct page{
    long int *virtual_address_start; //32bits
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
	unsigned int length;
	unsigned int length_high;
	unsigned int type;
	unsigned int pad;
}e820_entry;

typedef struct memory_entry {
	unsigned long start_addr;
	unsigned long length;
	short reserved;
}memory_entry;

struct contiguous_mem_entries{
	struct memory_entry entries[256]; //definitely no more than the e820 entries
} contiguous_mem_entries;

struct boot_param {
	struct e820_entry memMapp[256];
	unsigned long e820_num;
};

extern void boot_memory_init();


