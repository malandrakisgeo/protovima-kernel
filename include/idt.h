//lowlevel&osdev&minios&tinyos
#include "kernel.h"

/* 
    Ena IDT entry antistoixizei kathe interrupt se ena Interrupt Service Routine (ISR). 
    Perissotera: https://alex.dzyoba.com/blog/os-interrupts/
*/
typedef struct {
	uint16    isr_low;      // The lower 16 bits of the ISR's address
	uint16    kernel_cs;    // The GDT segment selector that the CPU will load into CS before calling the ISR
	uint8     reserved;     // Set to zero
	uint8     attributes;   // Type and attributes; see the IDT page
	uint16    isr_high;     // The higher 16 bits of the ISR's address
} __attribute__((packed)) idt_entry_t;
//Attribute packed: https://stackoverflow.com/questions/11770451/what-is-the-meaning-of-attribute-packed-aligned4
//safety:https://stackoverflow.com/questions/8568432/is-gccs-attribute-packed-pragma-pack-unsafe

__attribute__((aligned(0x10))) 
static idt_entry_t idt[256]; // Create an array of IDT entries; aligned for performance

typedef struct {
	uint16	limit; //size of IDT 
	uint32	base;	//The IDT's memory address 
} __attribute__((packed)) idtr_t;

static idtr_t idtr;

extern void initialize_idt();