char *concat(char dest[], char src[]);
char *char_append1(char dest[], char src);
char *terminal_char_append(char dest[], char src);

char *clear_char_array(char *dest);

char *deepcopy_char_array2(char *src, char *dest);
char *deepcopy_char_array(char dest[], char src[]);
//The cursor is already disabled via a BIOS function, but perhaps I'll switch to this method someday
void disable_cursor();
