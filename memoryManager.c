    

#include "kernel.h"
#include "types.h"
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

//int memMapLength=0

int *memMapLength = (int *) E820_ADDRESS; //Pointing to an address
unsigned char *rc ;
unsigned char *rc1 ;
unsigned long long availableMemBytes=0;



memoryMapEntry* memMapArr= (memoryMapEntry*)(E820_ADDRESS);

struct boot_param *boot_param = (struct boot_param *)(E820_ADDRESS);
struct boot_param *boot_paramvaddr = (struct boot_param *)VADDR(E820_ADDRESS);
struct boot_param *boot_paramvaddr2 = (struct boot_param *)VADDR2(E820_ADDRESS);

    //struct boot_param *boot_param = (struct boot_param *) E820_ADDRESS;


/* unsigned int hex_to_int(unsigned char cha){
        uint32 c  = 0;
        c  |= cha;
        //c/=16;
        unsigned int first = int first (cha / 16) - 3;          /// int first (c / 16) - 3 TA GAMAEI OLA! MEN HVORFOR?
        unsigned int second = (first % 16);         //epishs, an to c einai char, prepei to int na ginei uint8, alla kai tote enw den krasarei, h othoni ginetai katamavrh
        unsigned int result = (first*10) + second; //Apanthsh: Ta gamei ola epeidh ginetai memory overflow ston qemu, pou  telika odhgei se triple fault. Kai ginetai logw lathous sto linking. 
        if(result > 9){                             //To linking me cat fainotan eksarxhs polu paraplhgiko, alla esu proxwrhses me auto parauta. Xase twra poses wres apo th zwh sou, malaka. 
                                                   
            result--;
        } 
        return result;
    } 

*/

int hex_to_int(char c){
        int first = c / 16 - 3;
        int second = c % 16;
        int result = first*10 + second;
        if(result > 9) result--;
        return result;
}

int hex_to_ascii(char c, char d){
        int high = hex_to_int(c) * 16;
        int low = hex_to_int(d);
        return high+low;
}






 char hexdigit2int( char xd)
{
  if (xd <= '9'){
    return xd - '0';}
  
  if (xd == 'A')
    return 10;
  if (xd == 'B')
    return 11;
  if (xd == 'C')
    return 12;
  if (xd == 'D')
    return 13;
  if (xd == 'E')
    return 14;
  if (xd == 'F')
    return 15;
  return 0; 
  
}

void fixme(char *src) //TODO: Fix this.
{
 //char st[] = "99AB";
 //char *src = st;
   //char text[sizeof(st) + 1];
   char text[sizeof(&src) + 1];
   char *dst = text;

  while (*src != '\0')
  {
     unsigned char high = hexdigit2int(*src++);
     unsigned char low  = hexdigit2int(*src++);
    *dst++ = (high << 4) | low;
    //printlnVGA(*dst);
  }
  *dst = '\0';
  //printlnVGA(*dst);
}



void showAvailableMemory(){

    //fixme(p2);

    struct boot_paranm *p3 =  (struct boot_paranm*)(0x000002fd0);
    unsigned int memory_end =  0;
    //    char ee = (char ) 2 + '0';

    //int cnt = *(int *) (0x00000dfd0);
    //char cnchar =   (char ) cnt + '0';
    //println(&cnchar);
    //struct e820_entry *entry = p3->e820_list;

     int num = p3->e820_num;
         char cncharr =   (char ) num + '0';
    println(&cncharr);

    /* for(int i =0; i<num; i++){
      	if (entry[i].type != 1)
			    continue;
        if (entry[i].addr + entry[i].length > memory_end){
			    memory_end =  entry[i].length;}
    }*/

   unsigned int npage = memory_end / 0x1000;
    //char ee = (char ) &p3->memMapp[7].type + '0';
    //char cnchar =   (char ) npage + '0';
    //println(&cnchar);




}

void boot_memory_init(void)
{
	struct e820_entry *entry = boot_param->memMapp;
	int i, npage, num = boot_param->e820_num;

	/*
	 * We reserve memory below 1MB and use 820 map table
	 * to find end of memory, ignoring real maps.
	 */
	int memory_end = 0;
	printlnVGA("Memory map: ");
	//printk(" %-10s|%-10s|%-12s|\n", "addr", "length", "type");
	for (i = 0; i < num; i++) {
		//printk(" 0x%08x|0x%08x|%-12s|\n", entry[i].addr,
			//entry[i].length, e820_str[entry[i].type]);
		if (entry[i].type != E820_TYPE_FREE)
			continue;
		if (entry[i].addr + entry[i].length > memory_end)
			memory_end = entry[i].addr + entry[i].length;
	}
	/* caculate the number of memory pages */
	npage = memory_end / PGSIZE;
	//printk("memory size: %dKB(%dMB)\n", npage * 4, npage * 4 / 1024);

  unsigned char * str; 
  str = itoa(npage * 4 / 1024, str, 10);
  printlnVGA(str);
}


void showHex(){

     char *p2 =  ( int *) (0x00000dfd0);
     char *p3 =  ( char *)0x1c00; 
    printlnVGA(p2);

     //char p = &p2;
    //printlnVGA(p2);

    boot_memory_init();

}


