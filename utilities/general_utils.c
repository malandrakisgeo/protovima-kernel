char* concat(char dest[], char src[])
{
   int i = 0, j = 0;
   while (dest[i]) ++i;
   while (src[j]) dest[i++] = src[j++];
   dest[i] = '\0';
   return dest;
}

char* char_append1(char dest[], char src)
{
   int i = 0, j = 0;
   while (dest[i]) ++i;

   dest[i] = src;

   //dest[++i] = '\0';
   return dest;
}

char* char_append(char dest[], char src)
{
   int i = 0, j = 0;
   while (dest[i] != 0 && dest[i] !='0x00') ++i;

   dest[i] = src;

   //dest[++i] = '\0';
   return dest;
}


char* clear_command(char dest[])
{
   int i = 0, j = 0;
   while (dest[i] != 0 && dest[i] !='0x00'){
      dest[i] = 0x00;
      ++i;
   } 

   //dest[++i] = '\0';
   return dest;
}

//The cursor is already disabled via a BIOS function, but perhaps I'll switch to this method someday
void disable_cursor()
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}