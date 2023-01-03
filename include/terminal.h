#pragma once
#include "process.h"

char* fetch_args(char inserted_chars[], int cmd_name_ending_position);
void run_foreground_process(char * args);
void run_command(unsigned char * inserted_chars);
void receive_input(char ch);
void sample_command();
void dample_command(char *ch);
void malloc_command(char *size);
void clear_terminal_command();
void register_commands();
void start_terminal();