
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

//TODO: Ftiakse kati antistoixo tou OOM killer gia th periptwsh pou h diathesimh mnhmh peftei se epipeda katw tou 5%

static unsigned int max_pages;
static volatile struct contiguous_mem_entries *avail_mem_entries;
static struct e820_entry *entry;
static struct boot_param *myboot_param;
static unsigned long available_unpaged_memory = 0;

//unsigned long unavailable_memory = 0;
//unsigned long memory = 0;
static volatile unsigned long total_ram = 0;
static long memory_end = 0;

static volatile contiguous_mem_struct *next_free_page;

void boot_memory_init()
{

  volatile int contiguous_memory_total_entries = 0;
  volatile unsigned long i, num;

  unsigned char *str; //for the itoa

  myboot_param = (struct boot_param *)(E820_ADDRESS);

  entry = &myboot_param->memMapp;

  num = &myboot_param->e820_num; //apo th stigmh pou to myboot_param einai sto bss, prepei kai to num na einai sto bss. Eidallws den emfanizetai swsta.
  //TODO: Des an to parapanw pianetai gia normal

  /*
	 * We reserve memory below 1MB and use 820 map table
	 * to find end of memory, ignoring real maps.
	 */
  avail_mem_entries = (struct contiguous_memory_entries *)(entry[0].addr); //TODO: This is extremely bug-prone!

  for (i = 0; i < num; i++)
  {
    if (entry[i].type != E820_TYPE_FREE)
    { //TODO: Des ti sumvainei me th mh-diathesimh mnhmh kai giati mexri 3gb einai diathesima enw ta alla desmevontai.
      //unavailable_memory +=  entry[i].length; //debugging purposes
      continue;
    }
    // unsigned long mm =;

    //str = itoa(total_ram / (1024*1024), str, 10);
    //printlnVGA(str);

    if (entry[i].addr > PROCESS_MEMORY_START_ADDRESS)
    {
      avail_mem_entries->entries[contiguous_memory_total_entries].start_addr = entry[i].addr;
      avail_mem_entries->entries[contiguous_memory_total_entries].length = entry[i].length;
      ++contiguous_memory_total_entries;
      available_unpaged_memory += entry[i].length;
    }

    //unsigned long a = entry[i].addr + entry[i].length;
    if (entry[i].addr + entry[i].length > memory_end)
    {
      memory_end = entry[i].addr + entry[i].length;
    }
    total_ram += (entry[i].length);
  }

  printlnVGA("Available memory for processes: ");
  str = itoa(available_unpaged_memory / (1024 * 1024), str, 10);
  printlnVGA(str);

  printlnVGA("Total E820_TYPE_FREE memory: ");
  str = itoa(total_ram / (1024 * 1024), str, 10); //in mbs
  printlnVGA(str);

  /* caculate the number of memory pages for processes */
  max_pages = available_unpaged_memory / PAGE_SIZE;

  printlnVGA("Maximum number of pages for processes: ");
  str = itoa(max_pages, str, 10); //pages
  printlnVGA(str);

  printlnVGA("MEMEND address: ");
  str = itoa(memory_end, str, 10);
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
  /* unsigned char *str; //for the itoa
  str = itoa(avail_mem_entries->entries[0].length / (1024), str, 10); 
  printlnVGA(str);*/

  available_unpaged_memory -= 196608;
  available_unpaged_memory -= 65535;

  return avail_mem_entries->entries[0].start_addr; //adressen som ska användas till vår första page-tabell
}

void initialize_paging()
{
  /*

    For the time being we only have a single page table with 1024 entries of 4MB pages, 1*1024*4*48=196608bits=192kbs for the pages
    plus 1024*2*32 for the table itself

    Future plan: 
    The paging will partly rely on the 2004 linux implementetion (e.g https://lwn.net/Articles/106177/)
    Since 32bit addresses are used, 12 bits are used as offset to the physical address itself, and 20 for the page tables. 

    The first page table (3rd level) contains exactly four references to other tables -one for every GB of memory. 
    The index on the first table is referenced by the bits 31&30 of the address.

    Each of the four tables of the 2nd level contain 256 references to other tables (1st level).
    The index here is specified by the bits 29-22

    And the 1st level tables contain references to frame addresses -1024 each. 
    Referenced by the bits 21-12 of the address.

    A page table reference by itself needs 48bits of memory (int&short int variables). 
 
    In total we have 48*1024*256*4=50331648 bits for the paging, or 6144kb memory. 


    
  */
  int page_table_address = page_memory_alloc();
  contiguous_mem_struct *freepage = (struct free_page *)(page_table_address);
  contiguous_mem_struct *free_page_head = freepage;
  contiguous_mem_struct *current_free_page = freepage;

  for (int i = 1; i <= current_page_table_size; i++)
  {
    current_free_page->unused_page = (struct page *)(page_table_address + FREE_PAGE_TABLE_SIZE + (i * PAGE_SIZE)); //address of the next page
    current_free_page->unused_page->virtual_address_start = current_free_page->unused_page;                        //same as physical. TODO: fix it
    current_free_page->next = (struct free_page *)(page_table_address + (i * FREE_PAGE_STRUCTURE_SIZE));

    current_free_page = current_free_page->next;
    available_unpaged_memory -= PAGE_SIZE;
  }

  current_free_page->next = 0; //The last available free_page does not point somewhere.
  next_free_page = free_page_head;
}

void *page_alloc()
{
  contiguous_mem_struct *page_entry_to_be_used = next_free_page;

  next_free_page = next_free_page->next;

  page_entry_to_be_used->next = NULL;
  if (next_free_page->next == NULL)
  {
    printlnVGA("NO FREE PAGE");
  }

  return page_entry_to_be_used; //prev. page_entry_to_be_used->unused_page
}

//Allocates a contiguous chunk of memory -i.e. page entries for a process.

void *malloc(long size)
{

  long a = size / PAGE_SIZE;
  int b = size % PAGE_SIZE;

  if (b > 0)
  {
    a++;
  }

  //TODO: Elegxe an uparxei diathesimh mnhmh sto dothen size eksarxhs
  volatile contiguous_mem_struct *allocated_memory = (struct contiguous_mem_struct *)page_alloc(); //first entry
  volatile contiguous_mem_struct *entry = allocated_memory;                                        //an den uparxei enas epipleon pointer pros to arxiko allocated_memory, to first tha deixnei stongamo tou karagkiozh. O compiler lamvanei ta metra tou an uparxoun polloi pointer se mia dieuthinsh
  volatile contiguous_mem_struct *first = allocated_memory;
  //printitoa(first->unused_page->virtual_address_start, 10);// An to clang den exei -c kai mcmodel=large option, dhmiourgei provlhma.

  for (int i = 1; i < a; i++)
  { //runs only if at least 2 allocations are necessary
    allocated_memory->next = (struct contiguous_mem_struct *)page_alloc();
    allocated_memory = allocated_memory->next;
  }

  return first;
}

void dealloc(int address)
{
}
