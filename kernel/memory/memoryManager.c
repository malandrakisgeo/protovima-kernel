    
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


unsigned int max_pages;
struct e820_entry *entry; 
struct boot_param *myboot_param;
//struct boot_param *myboot_param= (struct boot_param *)(E820_ADDRESS);

unsigned long memory_end = 0;
unsigned long available_memory = 0;
unsigned long unavailable_memory = 0;


void boot_memory_init()
{
  myboot_param = (struct boot_param *)(E820_ADDRESS);

  unsigned char *str; //for the itoa
  /*int address = 0x00000dfd0;
  int* pcontent = (int*)address;
  int content = *pcontent;*/
  entry =  &myboot_param->memMapp;


  unsigned int i, num = &myboot_param->e820_num; //apo th stigmh pou to myboot_param einai sto bss, prepei kai to num na einai sto bss. Eidallws den emfanizetai.
  //TODO: Des an to parapanw pianetai gia normal

	/*
	 * We reserve memory below 1MB and use 820 map table
	 * to find end of memory, ignoring real maps.
	 */     

	for (i = 0; i < num; i++) {
		if (entry[i].type != E820_TYPE_FREE){ //TODO: Des ti sumvainei me th mh-diathesimh mnhmh kai giati mexri 3gb einai diathesima enw ta alla desmevontai.
      //unavailable_memory +=  entry[i].length; //debugging purposes
			continue;}
		if (entry[i].addr + entry[i].length > memory_end){
			memory_end = entry[i].addr + entry[i].length;}
    if (entry[i].addr > PROCESS_MEMORY_START_ADDRESS){
			available_memory +=  entry[i].length;
    }
	}
	printlnVGA("Available memory: ");
  str = itoa(available_memory / (1024*1024), str, 10); 
  printlnVGA(str);

  /*printlnVGA("Unavailable memory: ");
  str = itoa(unavailable_memory / (1024*1024), str, 10); 
  printlnVGA(str);
*/

  str = itoa(memory_end / (1024*1024), str, 10); //page in mbs
  printlnVGA(str);

  	/* caculate the number of memory pages for processes */
	max_pages = available_memory / PAGE_SIZE;

	printlnVGA("Maximum number of pages for processes: ");

  str = itoa(max_pages, str, 10); //pages
  printlnVGA(str);

  return;
}




void* page_memory_alloc(){
  /*
    Will diminish the available_memory while initializing paging.
    Will only be called once.
  */
}

void initialize_paging(){
  /*
    The paging relies partly on the 2004 linux implementetion (e.g https://lwn.net/Articles/106177/)
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
  page_memory_alloc();

}

void* malloc(int length){
  //will allocate one or more pages, depending on the length needed.

}

void dealloc(int* page){
  /*
    Deallocates a page.
  */
}


