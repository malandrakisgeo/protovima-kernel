#include "commands.h"

extern int foreground_process;
extern int calling_foreground_process;

extern char* char_append(char dest[], char src[]);
extern char* clear_command(char dest[]);

char command[32];
struct command_pointer cmds[32] ;


char fetch_args(char inserted_chars[], char args[], int cmd_name_ending_position){
    int i = cmd_name_ending_position, j=0;
    while(inserted_chars[i]!=0){
        args[j++] = inserted_chars[i++];
    }
}


void run_foreground_process(char args[]){
    if(foreground_process!=0){
        typedef void func();
        func* f = (func*)foreground_process;
        f(args);
    }
}

/*
    Ok, this isn't the best way to do it, but it works. 

    As a C-n00b with a Java background, I was unable to find an equivalent of a HashMap
    that would map the name of a command to a function pointer. 
    So I am doing a char-by-char comparison of the command entered and if some equivalent is found in the
    command_pointer struct, the corresponding function pointer is called.

    TODO: Learn better C and find a less retarded way to achieve this
*/
void run_command(char *inserted_chars){

    int cmd_not_found = 0;
    int j;
    for(int i =0; i<32; i++){
        j=0;
        cmd_not_found = 0;

        while(cmds[i].name[j] != 0 && cmd_not_found==0){
            if( (int)(cmds[i].name[j] ^ inserted_chars[j]) != 0){
                cmd_not_found=1;
            }
            j++;
        }

        if(!cmd_not_found){
            char args[32];


            void (*cmd)() = cmds[i].command_pointer; //function pointer
            calling_foreground_process = foreground_process;
            foreground_process = cmd;

            fetch_args(inserted_chars, args, j);
            run_foreground_process(args);
            foreground_process = calling_foreground_process; //return to terminal

            break;
        }
    }

    if(cmd_not_found==1){
            printlnVGA("Command not found."); 
            return;
    }


    return;

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

void dample_command(char ch[]){
    if(ch[0]!=0){
        printlnVGA("You inserted the arguments: ");
        printlnVGA(ch);
    }else{
        printlnVGA("No arguments inserted.");
    }

    return;
}




void register_commands(){
             typedef void func(char);
        func* dmp = (func*)dample_command;


    cmds[0].name = "sample";
    cmds[0].command_pointer = sample_command;
    cmds[1].name = "dample";
    cmds[1].command_pointer = dample_command;

}

/*
    It creates a struct that links command names to memory addresses.
*/

void start_terminal(){
    register_commands();

    void (*r_input)(char) = receive_input; //function pointer
    foreground_process = r_input;
    printlnVGA("TERMINAL RUNNING!"); 
    printchar('>');


}





/*
    Instead of writing multiple "if-else" statements for each and every available command, 
    we will write a single function that makes sure every command ends up running the function with its' name if it exists, otherwise prints and error.

*/
void link_commands_to_functions(){

}
