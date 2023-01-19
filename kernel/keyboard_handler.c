#include "keyboard_handler.h"
#include "vgaTextUtility.h"

int tableOffset = 0; //if caps lock or right shift, there will be an offset of 90

extern int foreground_process; //Zero if no foreground_process, equal to the address pointer of its' main function otherwise
//extern unsigned char *itoaa(unsigned long value, unsigned char *str, unsigned int base);

void send_to_foreground_process(char ch){
    if(foreground_process!=0){
        typedef void func(char);
        func* f = (func*)foreground_process;
        f(ch);
    }
}



void show_received_char(int char_pos){

    char str = keyboard_map[char_pos];
    if(foreground_process==0){ //If there is no foreground process running such as e.g. terminal
        printchar(str); 
    }else{
        send_to_foreground_process(str);
    }
    

}

//TODO: Make it less buggy
void general_keyboard_handler(unsigned int scancode){

    if(scancode == 54 ){ // if right shift pressed, but not released
        tableOffset = 90;
    }

    	// MSB set means key released
	if (scancode & 0x80){// Key released
    	scancode = scancode & 0x7F; //unset highest bit

        if(scancode == 54) { //right shift released
            tableOffset = 0;
        } else if(scancode == 58){ //Caps lock 
		    tableOffset = tableOffset^90; //Adds 90 if not zero, otherwise sets it to zero.
        } else {
            show_received_char(scancode + tableOffset);
        }
               
	}


}