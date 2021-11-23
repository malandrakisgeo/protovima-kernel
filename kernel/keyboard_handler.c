#include "keyboard_handler.h"

int tableOffset = 0; //if caps lock or right shift, there will be an offset of 90

void show_received_char(int char_pos){
    char str = keyboard_map[char_pos];
    printchar(str);
        //printlnVGA(itoa(char_pos,"",10));

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







    //if(!scancode & 0x80){
    //}

}