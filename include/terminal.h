#pragma once

char* fetch_args(char inserted_chars[], int cmd_name_ending_position);
char* terminal_char_append(char dest[], char src);

void run_foreground_process(char * args);
void find_and_create_process(unsigned char * inserted_chars);
void receive_input(char ch);
void sample_command();
void dample_command(char *ch);
void malloc_command(char *size);
void clear_terminal_command();
void register_commands();
void start_terminal_independently();
void start_terminal_proc();

void queue_current_process();