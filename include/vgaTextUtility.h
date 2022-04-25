//uint16* vga_buffer;

//unsigned int xPos;
//unsigned int yPos;


extern uint16 vga_entry(unsigned char ch, uint8 fore_color, uint8 back_color);

//clear video buffer array
extern void clear_vga_buffer(uint16 **buffer, uint8 fore_color, uint8 back_color);

//initialize vga buffer
extern void init_vga(uint8 fore_color, uint8 back_color);


extern unsigned char *itoa( unsigned long value, unsigned char * str, unsigned int base );


extern int println(unsigned char *text);

extern void printlnVGA(unsigned char *msg);

extern void printchVGA(unsigned char *msg);

extern void writechar(unsigned char c, unsigned char forecolour, unsigned char backcolour, int x, int y);

extern void writeline(unsigned char *msg);
