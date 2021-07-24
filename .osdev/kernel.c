/* #include <stdio.h>
 #include <sys/sysinfo.h>
#include <unistd.h>
#include <stdlib.h>*/

#define WHITE_TXT 0x07 /* light gray on black text */

void k_clear_screen();
unsigned int k_printf(char *message, unsigned int line);



/* simple kernel written in C */
void main() 
{
	k_clear_screen();
	k_printf("Hello, world! Welcome to my kernel.", 0);
};

/* k_clear_screen : to clear the entire text screen */
void k_clear_screen()
{
	char *vidmem = (char *) 0xb8000;
	unsigned int i=0;
	while(i < (80*25*2))
	{
		vidmem[i]=' ';
		i++;
		vidmem[i]=WHITE_TXT;
		i++;
	};
};

/* k_printf : the message and the line # */
unsigned int k_printf(char *message, unsigned int line)
{
	char *vidmem = (char *) 0xb8000;
	unsigned int i=0;

	i=(line*80*2);

	while(*message!=0)
	{
		if(*message=='\n') // check for a new line
		{
			line++;
			i=(line*80*2);
			*message++;
		} else {
			vidmem[i]=*message;
			*message++;
			i++;
			vidmem[i]=WHITE_TXT;
			i++;
		};
	};

	return(1);
}

/*void main() {
    /*const short color = 0x0F00;
    const char* hello = "Hello from cpp file!";
    short* vga = (short*)0xb1000;
    for (int i = 0; i<16;++i)
        vga[i+80] = color | hello[i];
			//clrscr();
	printf("Hellgfffffffffffo!");
	for(;;) {}
}*/

/* unsigned char in(unsigned short _port) {
	unsigned char result;
	__asm__ ("in %%dx, %%al" : "=a" (result) : "d" (_port));
	return result;
}

void out(unsigned short _port, unsigned char _data) {
	__asm__ ("out %%al, %%dx" : : "a" (_data), "d" (_port));
}

const unsigned char* SCREEN_ADDR = (unsigned char*) (0xB8000);

const unsigned int SCREEN_WIDTH = 80;
const unsigned int SCREEN_HEIGHT = 25;

const unsigned int INDEX_REGISTER = 0x3D4;
const unsigned int DATA_REGISTER = 0x3D5;


void clrscr() {
	unsigned char* vidmem = (unsigned char*) SCREEN_ADDR;
	const long size = SCREEN_WIDTH * SCREEN_HEIGHT;
	long loop;

	for(loop = 0; loop < size; loop++) {
		*vidmem++ = 0;
		*vidmem++ = 0xF;
	}
	out(INDEX_REGISTER, 14);
	out(DATA_REGISTER, 0);
	out(INDEX_REGISTER, 15);
	out(DATA_REGISTER, 0);
}

void print(const char* _message) {
	unsigned char* vidmem = (unsigned char*) SCREEN_ADDR;
	unsigned short offset;
	unsigned long i;

	out(INDEX_REGISTER, 14);
	offset = in(DATA_REGISTER) << 8;
	out(INDEX_REGISTER, 15);
	offset |= in(DATA_REGISTER);

	vidmem += offset * 2;
	i = 0;
	while(_message[i] != 0) {
		*vidmem = _message[i++];
		vidmem += 2;
	}
	offset += i;
	out(DATA_REGISTER, (unsigned char) (offset));
	out(INDEX_REGISTER, 14);
	out(DATA_REGISTER, (unsigned char) (offset >> 8));
}*/