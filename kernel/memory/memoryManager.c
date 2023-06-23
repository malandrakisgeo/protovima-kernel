
#include "kernel.h"
#include "memoryManager.h"

/*
 PhysicalAddress = Segment * 16 + Offset.
 12F3:4B27 corresponds to the physical address 0x17A57
*/


static volatile unsigned int max_pages;
static volatile struct contiguous_mem_entries *avail_mem_entries;
static volatile struct e820_entry *entry;
static volatile struct boot_param *myboot_param;
static volatile unsigned long available_unpaged_memory = 0; // in bytes

// unsigned long unavailable_memory = 0; //debugging purposes
// unsigned long memory = 0;
static volatile unsigned long total_ram = 0;
static volatile  long memory_end = 0;

static contiguous_mem_struct *next_free_page;

void boot_memory_init()
{
  unsigned char *str; // for the int_to_char


  volatile int contiguous_memory_total_entries = 0;
  volatile unsigned long i, num;


  myboot_param = (struct boot_param *)(E820_ADDRESS);

  entry = &myboot_param->memMapp;

  num = &myboot_param->e820_num; 

  /*
   * We reserve memory below 1MB and use 820 map table
   * to find end of memory, ignoring real maps.
   */
  avail_mem_entries = (struct contiguous_memory_entries *)(entry[0].addr); // TODO: This is extremely bug-prone!

  for (i = 0; i < num; i++)
  {
    if (entry[i].type != E820_TYPE_FREE)
    { 
      // unavailable_memory +=  entry[i].length; //debugging purposes
      continue;
    }

    if (entry[i].addr > PROCESS_MEMORY_START_ADDRESS)
    {
      avail_mem_entries->entries[contiguous_memory_total_entries].start_addr = entry[i].addr;
      avail_mem_entries->entries[contiguous_memory_total_entries].length = entry[i].length;
      ++contiguous_memory_total_entries;
      available_unpaged_memory += entry[i].length;
    }

    // unsigned long a = entry[i].addr + entry[i].length;
    if (entry[i].addr + entry[i].length > memory_end)
    {
      memory_end = entry[i].addr + entry[i].length;
    }
    total_ram += (entry[i].length);
  }

  printlnVGA("Available memory for processes: ");
  str = int_to_char(available_unpaged_memory / (1024 * 1024), str, 10);
  printlnVGA(str);

  printlnVGA("Total E820_TYPE_FREE memory: ");
  str = int_to_char(total_ram / (1024 * 1024), str, 10); // in mbs
  printlnVGA(str);

  /* caculate the number of memory pages for processes */
  max_pages = available_unpaged_memory / PAGE_SIZE;

  printlnVGA("Maximum number of pages for processes: ");
  str = int_to_char(max_pages, str, 10); // pages
  printlnVGA(str);

  printlnVGA("MEMEND address: ");
  str = int_to_char(memory_end, str, 10);
  printlnVGA(str);
  
  return;
}

void *page_memory_alloc()
{
  /*
    Will diminish the available_memory while initializing paging.
    Will only be called once.

    For the time being we only have a page table with 1024 entries of 4MB pages, 1*1024*4*48=196608bits=192kbs for the pages,
    1024*2*32 for the page_table

  */

  available_unpaged_memory -= (FREE_PAGE_TABLE_SIZE/8) ;//bits to bytes. 196608;
  available_unpaged_memory -= 65535;

  return avail_mem_entries->entries[0].start_addr; //adressen som ska användas till vår första page-tabell
}

void initialize_paging()
{
  long int page_table_address = page_memory_alloc();
  contiguous_mem_struct *freepage = (struct contiguous_mem_struct *)(page_table_address);
  contiguous_mem_struct *free_page_head = freepage;
  contiguous_mem_struct *current_free_page = freepage;


  for (int i = 1; i <= current_page_table_size; i++)
  {
    current_free_page->unused_page = (struct page *)(page_table_address + FREE_PAGE_TABLE_SIZE + (i * PAGE_SIZE)); // address of the next page
    current_free_page->unused_page->start_address = &current_free_page->unused_page->start_address;                                // same as physical. TODO: fix it
    current_free_page->next = (struct contiguous_mem_struct *)(page_table_address + (i * CONT_MEM_STRUCT_SIZE));

    current_free_page = current_free_page->next;
    available_unpaged_memory -= PAGE_SIZE;
  }

  current_free_page->next = NULL; // The last available free_page does not point somewhere.
  next_free_page = free_page_head;
  printlnVGA("Paging initialized.");
  return;
}

void *page_alloc()
{
  if (available_unpaged_memory < PAGE_SIZE)
  {
    printlnVGA("NOT ENOUGH MEMORY!");
    return 0;
  }

  if (next_free_page->next == NULL)
  {
    printlnVGA("NO FREE PAGE LEFT");
    return 0;
  }
  available_unpaged_memory -= PAGE_SIZE;
  contiguous_mem_struct *page_entry_to_be_used = next_free_page;

  next_free_page = next_free_page->next;

  page_entry_to_be_used->next = NULL;
  page_entry_to_be_used->next->next = NULL;
  page_entry_to_be_used->next->unused_page = NULL;

  return page_entry_to_be_used; // prev. page_entry_to_be_used->unused_page

}

/*
  Allocates a contiguous chunk of memory -i.e. page entries for a process.
  I was reluctant to call it malloc, since malloc is a C-library function that relies on system calls
  to the underlying OS in order to get memory, not a kernel functionality itself.

  This can either be called by a process (future plan), or manually by a user for testing.
  In the latter case, some output messages are printed.

*/
volatile void *malloc(long size_in_bytes, int called_by_process)
{
    __asm__ __volatile__   ("cli\n\t");

  if (available_unpaged_memory < size_in_bytes)
  {
    printlnVGA("NOT ENOUGH MEMORY!");
    return 0;
  }

  long total_pages = size_in_bytes / PAGE_SIZE; //num of pages
  int b = size_in_bytes % PAGE_SIZE; //possible extra page

  if (b > 0)
  {
    total_pages++;
  }
  contiguous_mem_struct *allocated_memory = (struct contiguous_mem_struct *)page_alloc(); // first entry

  contiguous_mem_struct *first_allocated_block = allocated_memory;
  // printitoa(first->unused_page->start_address, 10);// An to clang den exei -c kai mcmodel=large option, dhmiourgei provlhma.

  for (int i = 1; i < total_pages; i++)
  { // runs only if at least 2 allocations are necessary
    allocated_memory->next = (struct contiguous_mem_struct *)page_alloc();
    allocated_memory = allocated_memory->next;
  }

  contiguous_mem_struct *last_allocated_block = first_allocated_block; //uses the first as a starting point

  while (last_allocated_block->next != NULL)
  {
    last_allocated_block = last_allocated_block->next;
  }

  if (called_by_process == 0) // show these messages only if the user called it directly as a command
  { 
    printlnVGA("Physical addresses of the first pages (at most 3): ");
    print_int_as_char(first_allocated_block->unused_page->start_address, 10);
    if (first_allocated_block->next != 0)
    {
      print_int_as_char(first_allocated_block->next->unused_page->start_address, 10);
      if (first_allocated_block->next->next != 0)
      {
        print_int_as_char(first_allocated_block->next->next->unused_page->start_address, 10);
      }
    }
    printlnVGA("Last page: ");
    print_int_as_char(last_allocated_block->unused_page->start_address, 10);
  }
      __asm__ __volatile__   ("sti\n\t");

  return first_allocated_block;
}

void dealloc(contiguous_mem_struct *allocated_mem_struct)
{

  while (allocated_mem_struct->next != 0)
  {
    next_free_page->next = allocated_mem_struct;
    available_unpaged_memory += PAGE_SIZE;
    allocated_mem_struct = allocated_mem_struct->next;
  }

  next_free_page->next = allocated_mem_struct;
  available_unpaged_memory += PAGE_SIZE;
}
