#include "terminal.h"

extern int foreground_process;
extern int calling_foreground_process;

extern char *char_append(char dest[], char src[]);
extern char *clear_command(char dest[]);
extern void *malloc(long a, int b);
extern  void printlnVGA(unsigned char *msg);

char command[32];
struct command_pointer cmds[32];
 char args[32];

char* fetch_args(char inserted_chars[], int cmd_name_ending_position)
{
    int i = cmd_name_ending_position, j = 0;

    while (inserted_chars[i] != 0)
    {
        args[j++] = inserted_chars[i++];
    }

    return args;
}

//Calls a function pointer with an array of arguments
void run_foreground_process(char * args)
{
/*if (foreground_process != 0)
    {
        typedef void func();
        func *f = (func *)foreground_process;
        f(&args);
    }*/
   
    if (foreground_process != 0){
        pv_process pp = create_process((void*)foreground_process);
        run_process(&pp, args);
    }
    return;

}

/*
    Ok, this isn't the best way to do it, but it works. 

    As a C-n00b with a Java background, I was unable to find an equivalent of a HashMap
    that would map the name of a command to a function pointer. 
    So I am doing a char-by-char comparison of the command entered and if some equivalent is found in the
    command_pointer struct, the corresponding function pointer is called.

    TODO: Learn better C and find a less retarded way to achieve this
*/
void run_command(unsigned char * inserted_chars)
{

    int cmd_not_found;
    int j;
    for (int i = 0; i < 32; i++)
    {
        j = 0;
        cmd_not_found = 0;

        /*
            The while-loop below takes care of the comparison.

        */
        while (cmds[i].name[j] != 0 && cmd_not_found == 0)
        {
            if ((int)(cmds[i].name[j] ^ inserted_chars[j]) != 0)
            {
                cmd_not_found = 1;
            }
            j++;
        }

        if (!cmd_not_found)
        { //if command found

            void (*cmd)() = cmds[i].command_pointer;         //find the function pointer to the command
            calling_foreground_process = foreground_process; //store the address of the terminal for returning
            foreground_process = cmd;                        //and bring the command to the foreground.
            //Keep in mind that as of 5/2022 this project does not support parallelism and at most one process can run at a time
  
            char *args = fetch_args(inserted_chars, j);      //get the arguments after the command
            run_foreground_process(args);                    //and call the command with them
            foreground_process = calling_foreground_process; //return to terminal

            break;
        }
    }

    if (cmd_not_found == 1)
    {
        printlnVGA("Command not found.");
    }

    return;
}

void receive_input(char ch)
{
    if (ch != '\n')
    {
        char_append(command, ch);
        printchar(ch);
    }
    else
    {
        run_command(command);
        clear_command(command);
        printchar('>');
    }
}

void sample_command()
{
    printlnVGA("Sample command ran!");
    return;
}

void dample_command(char *ch)
{
    if (ch[0] != 0)
    {
        printlnVGA("You inserted the arguments: ");
        printlnVGA(ch);
    }
    else
    {
        printlnVGA("No arguments inserted.");
    }

    return;
}

void malloc_command(char *size)
{   
    int rc = 0;
    unsigned i = 1; //0 is a space
    // C guarantees that '0'-'9' have consecutive values
    while (size[i] != 0 && size[i] != '0x00')
    {

        if (size[i] >= 48 && size[i] <= 57)
        {
            rc *= 10;
            rc += (size[i] - 48);
        }
        ++i;
    }

    malloc(rc, 0);

    return;
}

void clear_terminal_command()
{
    clear();
    return;
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
}

/*
    It creates a struct that links command names to memory addresses.
*/

void start_terminal()
{
    register_commands();

    void (*r_input)(char) = receive_input; //function pointer
    foreground_process = (int )r_input;
    printlnVGA("TERMINAL RUNNING!");
    printchar('>');
}

