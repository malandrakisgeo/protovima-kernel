#include "commands.h"

extern int foreground_process;
extern int calling_foreground_process;

extern char* char_append(char dest[], char src[]);
extern char* clear_command(char dest[]);

char command[32];
struct command_pointer cmds[32] ;


void run_foreground_process(){
    if(foreground_process!=0){
        typedef void func();
        func* f = (func*)foreground_process;
        f();
    }
}

/*
    Ok, this isn't an efficient way to do it, but it works. 

    As a C-n00b with a Java background, I am unable to find an equivalent of a HashMap
    that would map the name of a command to a function pointer. 
    So I am doing a char-by-char comparison of the command entered and if some equivalent is found in the
    command_pointer struct, the corresponding function pointer is called.

    TODO: Learn better C and find a less retarded way to achieve this
*/
void run_command(char *ch){

    int cmd_not_found = 0;

    for(int i =0; i<3; i++){
        int j=0;
        cmd_not_found = 0;

        while(cmds[i].name[j] != 0 && cmd_not_found==0){
            if( (int)(cmds[i].name[j] ^ ch[j]) != 0){
                cmd_not_found=1;
            }
            j++;
        }

        if(!cmd_not_found){
            void (*cmd)() = cmds[i].command_pointer; //function pointer
            calling_foreground_process = foreground_process;
            foreground_process = cmd;
            run_foreground_process();
            foreground_process = calling_foreground_process; //return to terminal

            return;
        }
    }

    if(cmd_not_found==1){
            printlnVGA("Command not found."); 
            return;
    }

}

void receive_input(char ch){
        if(ch!='\n'){
            char_append(command, ch);
            printchar(ch);
        }else{
            //printlnVGA(ch);
            //printlnVGA(command);
            run_command(command);
            clear_command(command);
            printchar('>');
        }
    
}

void sample_command(){
    printlnVGA("Sample command ran!");
    return;
}

void dample_command(){
    printlnVGA("Another command ran");
    return;
}

/*
    It creates a struct that links command names to memory addresses.
*/

void start_terminal(){
    void (*r_input)(char) = receive_input; //function pointer
    foreground_process = r_input;
    printlnVGA("TERMINAL RUNNING!"); 
    printchar('>');


    void (*c_com)(char) = sample_command; //function pointer

    cmds[0].name = "sample";
    cmds[0].command_pointer = sample_command;
    cmds[1].name = "dample";
    cmds[1].command_pointer = dample_command;


}

/*
    Instead of writing multiple "if-else" statements for each and every available command, 
    we will write a single function that makes sure every command ends up running the function with its' name if it exists, otherwise prints and error.

*/
void link_commands_to_functions(){

}
