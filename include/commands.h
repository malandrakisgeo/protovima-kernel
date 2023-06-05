#pragma once


struct command_pointer{
    char* name;
	int *command_pointer;
};

void halt();
void cpu_info();

void inf_loop();