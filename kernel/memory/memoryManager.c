
#include "kernel.h"
#include "memoryManager.h"

/*

    Ksekiname me available-memory kai base_address ths.

    Gia kathe malloc desmevoume to antistoixo kommati
    mnhmhs ksekinwntas apo base_address kai to afairoume apo available-memory.
    An ftasoume sto final_address kai den uparxei kai available-memory
    , petaei error. An uparxei... desmevoume to "akriano" kommati mnhmhs
    kai pame pisw sto base_address psaxnontas kati eleuthero...

    Menei omws to pws psaxnoume ta eleuthera kommatia. Me brute-force? Mhpws einai polus xronos
    gia p.x. 32 gb?
    H mhpws na dhmiourghsoume ena array gia ta apodesmeuthenta kommatia
     mnhmhs wste na desmeuoume kateutheian apo ekei? Auto to array tha sunista mia apoklish
     alla oxi gia polu, afou kathe malloc/calloc tha koitazei prwta ekei pera.

    Fun fact: Kathe mem_entry pianei mnhmh, opote prwta to dhmiourgoume kai meta rwtame
    posh mnhmh exei eleutherh. Timh sto mem_entry.address ktl dinoume meta.

*/

/*
 PhysicalAddress = Segment * 16 + Offset.
 12F3:4B27 corresponds to the physical address 0x17A57
*/

// TODO: Ftiakse kati antistoixo tou OOM killer gia th periptwsh pou h diathesimh mnhmh peftei se epipeda katw tou 5%

static unsigned int max_pages;
static struct contiguous_mem_entries *avail_mem_entries;
static struct e820_entry *entry;
static struct boot_param *myboot_param;
static unsigned long available_unpaged_memory = 0; // in bytes

// unsigned long unavailable_memory = 0;
// unsigned long memory = 0;
static unsigned long total_ram = 0;
static long memory_end = 0;

static contiguous_mem_struct *next_free_page;

void boot_memory_init()
{

  volatile int contiguous_memory_total_entries = 0;
  volatile unsigned long i, num;

  unsigned char *str; // for the itoaa

  myboot_param = (struct boot_param *)(E820_ADDRESS);

  entry = &myboot_param->memMapp;

  num = &myboot_param->e820_num; // apo th stigmh pou to myboot_param einai sto bss, prepei kai to num na einai sto bss. Eidallws den emfanizetai swsta.
  // TODO: Des an to parapanw pianetai gia normal

  /*
   * We reserve memory below 1MB and use 820 map table
   * to find end of memory, ignoring real maps.
   */
  avail_mem_entries = (struct contiguous_memory_entries *)(entry[0].addr); // TODO: This is extremely bug-prone!

  for (i = 0; i < num; i++)
  {
    if (entry[i].type != E820_TYPE_FREE)
    { // TODO: Des ti sumvainei me th mh-diathesimh mnhmh kai giati mexri 3gb einai diathesima enw ta alla desmevontai.
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
  str = itoaa(available_unpaged_memory / (1024 * 1024), str, 10);
  printlnVGA(str);

  printlnVGA("Total E820_TYPE_FREE memory: ");
  str = itoaa(total_ram / (1024 * 1024), str, 10); // in mbs
  printlnVGA(str);

  /* caculate the number of memory pages for processes */
  max_pages = available_unpaged_memory / PAGE_SIZE;

  printlnVGA("Maximum number of pages for processes: ");
  str = itoaa(max_pages, str, 10); // pages
  printlnVGA(str);

  printlnVGA("MEMEND address: ");
  str = itoaa(memory_end, str, 10);
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
  printlnVGA("Paging initialized. ");
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
void *malloc(long size_in_bytes, int called_by_process)
{

  if (available_unpaged_memory < size_in_bytes)
  {
    printlnVGA("NOT ENOUGH MEMORY!");
    return 0;
  }

  long a = size_in_bytes / PAGE_SIZE;
  int b = size_in_bytes % PAGE_SIZE;

  if (b > 0)
  {
    a++;
  }
  contiguous_mem_struct *allocated_memory = (struct contiguous_mem_struct *)page_alloc(); // first entry

  contiguous_mem_struct *first = allocated_memory;
  // printitoa(first->unused_page->start_address, 10);// An to clang den exei -c kai mcmodel=large option, dhmiourgei provlhma.

  for (int i = 1; i < a; i++)
  { // runs only if at least 2 allocations are necessary
    allocated_memory->next = (struct contiguous_mem_struct *)page_alloc();
    allocated_memory = allocated_memory->next;
  }

  contiguous_mem_struct *last = first;

  while (last->next != NULL)
  {
    last = last->next;
  }

  if (called_by_process == 0)
  { // show messages only if the user called it directly as a command
    printlnVGA("Physical addresses of the first pages (at most 3): ");
    printitoa(first->unused_page->start_address, 10);
    if (first->next != 0)
    {
      printitoa(first->next->unused_page->start_address, 10);
      if (first->next->next != 0)
      {
        printitoa(first->next->next->unused_page->start_address, 10);
      }
    }
    printlnVGA("Last page: ");
    printitoa(last->unused_page->start_address, 10);
  }

  return first;
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
