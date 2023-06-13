#include "terminal.h"
#include "vgaTextUtility.h"

extern int foreground_process;
extern int calling_foreground_process;


char command[32];
struct command_pointer cmds[32];
char args[32];
char previous_command;
int previous_command_pointer=0;
int command_history=0;
static int inexistent_command =0 ;

void command_history_handler(){
    deepcopy_char_array(command, previous_command);
   
    return;
} 

void show_previous_command(){
    if(previous_command_pointer!=0){
        //do nothing.
    }else{
        remove_written_message_before_newline(command);
        deepcopy_char_array(previous_command, command);
        print_msg(command);

        ++previous_command_pointer;
    }
    return;
}

char* fetch_args(char inserted_chars[], int cmd_name_ending_position)
{
    int i = cmd_name_ending_position, j = 0;

    while (inserted_chars[i] != 0)
    {
        args[j++] = inserted_chars[i++];
    }

    return args;
}

char *terminal_char_append(char dest[], char src)
{
   int i = 0, j = 0;
   while (dest[i] != 0 && dest[i] != '0x00')
      ++i;

   if(src != '\b'){ //backspace
         dest[i] = src;
   }else{
      dest[--i] = 0x00;
   }

   //dest[++i] = '\0';
   return dest;
}

/*
    Ok, this isn't the best way to do it, but it works. 

    As a C-n00b with a Java background, I was unable to find an equivalent of a HashMap
    that would map the name of a command to a function pointer. 
    So I am doing a char-by-char comparison of the command entered and if some equivalent is found in the
    command_pointer struct, the corresponding function pointer is called.

    TODO: Learn better C and find a less retarded way to achieve this
*/
void find_and_run(unsigned char * inserted_chars)
{
     
    int j, i;
    inexistent_command=1;
    for (i = 0; i < 32; i++) {
        if(cmds[i].command_pointer == 0){ //if undefined command
            break;
        }
        inexistent_command = 0;
        j = 0;

        /*
            The while-loop below takes care of the comparison.
        */
        while (cmds[i].name[j] != 0 && inexistent_command == 0 )  
        {
            if ((int)(cmds[i].name[j] ^ inserted_chars[j]) != 0)
            {
                inexistent_command = 1; 
            }
            j++;
        }

        if (inexistent_command == 0)
        { //if command found
            void (*cmd)() = cmds[i].command_pointer;         //find the function pointer to the command
            calling_foreground_process = foreground_process; //store the address of the terminal for returning
            foreground_process = cmd;                        //and bring the command to the foreground.
  
            char *args = fetch_args(inserted_chars, j);      //get the arguments after the command
            run_foreground_process(args);                    //and call the command with them
            foreground_process = calling_foreground_process; //return to terminal
            command_history_handler();
            previous_command_pointer=0;
            return;
        }

    }
    if (inexistent_command!=0){ //the loop has checked all commands and they are nothing like the one given.
        printlnVGA("Command not found.");
    }
     return;
}


void receive_input(char ch)
{
    if (ch != '\n' && ch != 24){ //newline or up
        terminal_char_append(command, ch);
        printchar(ch);
    }else if(ch == 24){ //arrow up
        show_previous_command(); //show the last command that ran successfully
    }else if(ch == '\n'){ //on enter
        printchar('\n');
        find_and_run(command);
        clear_char_array(command);
        clear_char_array(args);
        printchar('>');
    }
}


void register_commands()
{
    typedef void func(char);
    //func* dmp = (func*)dample_command;

    cmds[0].name = "sample";
    cmds[0].command_pointer = sample_command;
    cmds[1].name = "dample";
    cmds[1].command_pointer = dample_command;
    cmds[2].name = "malloc";
    cmds[2].command_pointer = malloc_command;
    cmds[3].name = "clear";
    cmds[3].command_pointer = clear_terminal_command;
    cmds[4].name = "cpuinfo";
    cmds[4].command_pointer = cpu_info;
    cmds[5].name = "infloop";
    cmds[5].command_pointer = inf_loop;
}

/*
    It creates a struct that links command names to memory addresses.
    This is meant to be run directly, without setting up the process management.
*/
void start_terminal_independently()
{
    register_commands();

    void (*r_input)(char) = receive_input; //function pointer
    foreground_process = (int )r_input;
    
    printlnVGA("TERMINAL RUNNING!");
    printchar('>');
}

void start_terminal_proc()
{
    register_commands();
    printlnVGA("TERMINAL RUNNING!");
    printchar('>');
}
