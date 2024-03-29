//https://stackoverflow.com/questions/37618111/keyboard-irq-within-an-x86-kernel

#pragma once

unsigned char keyboard_map[256] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',     /* 9 */
  '9', '0', '-', '=', '\b',     /* Backspace */
  '\t',                 /* Tab */
  'q', 'w', 'e', 'r',   /* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', /* Enter key */
    0,                  /* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',     /* 39 */
 '\'', '`',   0,                /* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',                    /* 49 */
  'm', ',', '.', '/',   0,                              /* Right shift */
  '*',
    0,  /* Alt */
  ' ',  /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    24,  /* Up Arrow */
    0,  /* Page Up */
  '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
  '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key 88*/
    0,  /* All other keys are undefined  89*/
        0,  0, 0,0,0,0,0,0,0,0,     /* If there is a shift or a caps lock, these will be used: */
  0,0,0,0,0,0,
    'Q', 'W', 'E', 'R',   
  'T', 'Y', 'U', 'I', 'O', 'P', 0, 0, '\n',0,
  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 0,    
 0, 0,   0,  0, 'Z', 'X', 'C', 'V', 'B', 'N',                    /* 49 */
  'M', ',', '.', '/',   0,                              
  '*', 0,  ' ',  
};

void show_received_char(int char_pos);

void general_keyboard_handler(unsigned int scancode);