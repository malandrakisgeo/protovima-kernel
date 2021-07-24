    
#include "kernel.h"
#include "memoryManager.h"

/*#include "vgaTextUtility.c"

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

//uint8_t availableMemory=0;
//a=&p;

//extern void do_e820(void);   //That's how we call code from assembly, afotou dhlwthei ws global

/* 
 PhysicalAddress = Segment * 16 + Offset.
 12F3:4B27 corresponds to the physical address 0x17A57
*/




//struct boot_param *myboot_param = (struct boot_param *)(E820_ADDRESS);



void boot_memory_init()
{

  struct boot_param *myboot_param = (struct boot_param *)(0x00000B00);



int address = 0x00000dfd0;
int* pcontent = (int*)address;
int content = *pcontent;
  printlnVGA(itoa(content, ' d', 10));



	struct e820_entry *entry = myboot_param->memMapp;
	unsigned long i, npage, num = &myboot_param->e820_num;

	/*
	 * We reserve memory below 1MB and use 820 map table
	 * to find end of memory, ignoring real maps.
	 */     

  printlnVGA(itoa(num, ' d', 10));
    unsigned char myString[] = "dd";
	printlnVGA(myString);

	unsigned long memory_end = 0;
	printlnVGA("Memory map: ");
	for (i = 0; i < num; i++) {
		if (entry[i].type != E820_TYPE_FREE)
			continue;
		if (entry[i].addr + entry[i].length > memory_end){
			memory_end = entry[i].addr + entry[i].length;}
	}
	/* caculate the number of memory pages */
	npage = memory_end / PGSIZE;
  //printlnVGA(itoa(npage, ' d', 10));

  unsigned char *str; 
  str = itoa(npage * 4 / 1024, str, 10); //page in mbs
        printlnVGA(str);

  return;
}




/*
PAGING: 

1. Tessera levels. Toutestin, tessera page tables, me 512 kataxwrhseis
twn 8 bytes to kathena

*/

