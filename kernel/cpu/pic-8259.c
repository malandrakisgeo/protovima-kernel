#include "pic-8259.h"
#include "inline-assembly.h"

//End of interrupt
void PIC_sendEOI(unsigned char irq)
{
	if(irq >= 8)
		outb(MASTER_PIC_COMMAND,PIC_EOI);
 
	outb(SLAVE_PIC_COMMAND,PIC_EOI);
}


//OSDEV.ORG
/*
arguments:
	offset1 - vector offset for master PIC
		vectors on the master become offset1..offset1+7
	offset2 - same for slave PIC: offset2..offset2+7
*/
void PIC_remap()
{
	unsigned char a1, a2;
 
	a1 = inb(MASTER_PIC_DATA);                        // save masks
	a2 = inb(SLAVE_PIC_DATA);
 
	outb(MASTER_PIC_DATA, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	io_wait();
	outb(SLAVE_PIC_DATA, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(MASTER_PIC_DATA, 0x0);                 // ICW2: Master PIC vector offset
	io_wait();
	outb(SLAVE_PIC_DATA, 0x0);                 // ICW2: Slave PIC vector offset
	io_wait();
	outb(MASTER_PIC_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	io_wait();
	outb(SLAVE_PIC_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	io_wait();
 
	outb(MASTER_PIC_DATA, ICW4_8086);
	io_wait();
	outb(SLAVE_PIC_DATA, ICW4_8086);
	io_wait();
 
	outb(MASTER_PIC_DATA, a1);   // restore saved masks.
	outb(SLAVE_PIC_DATA, a2); //The timer wouldn't work when it was 0xff. Works with 0x0

}


void initialize_pic()
{
    	unsigned char a1, a2;
 
	a1 = inb(MASTER_PIC_DATA);                        // save masks
	a2 = inb(SLAVE_PIC_DATA);
    /* ICW1 - begin initialization */
    outb(PIC_1_CTRL, 0x11);
    outb(PIC_2_CTRL, 0x11);

    /* ICW2 - remap offset address of idt_table */
    /*
    * In x86 protected mode, we have to remap the PICs beyond 0x20 because
    * Intel have designated the first 32 interrupts as "reserved" for cpu exceptions
    */
    outb(PIC_1_DATA, 0x20);
    outb(PIC_2_DATA, 0x28);

    /* ICW3 - setup cascading */
    outb(PIC_1_DATA, 0x04);
    outb(PIC_2_DATA, 0x02);

    /* ICW4 - environment info */
    outb(PIC_1_DATA, 0x01);
    outb(PIC_2_DATA, 0x01);
    /* Initialization finished */

    /* mask interrupts */
    outb(0x21 , a1); //The timer wouldn't work when it was 0xff. Works with 0x0
    outb(0xA1 , a2); //TODO: Read how it works and what purpose it serves
}


