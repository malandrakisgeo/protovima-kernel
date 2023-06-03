#include "general_utils.h"

char *concat(char dest[], char src[])
{
   int i = 0, j = 0;
   while (dest[i])
      ++i;
   while (src[j])
      dest[i++] = src[j++];
   dest[i] = '\0';
   return dest;
}

char *char_append1(char dest[], char src)
{
   int i = 0, j = 0;
   while (dest[i])
      ++i;

   dest[i] = src;

   //dest[++i] = '\0';
   return dest;
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

char *clear_char_array(char *dest)
{
   int i = 0, j = 0;
   while (dest[i] != 0x00)
   {
      dest[i++] = 0x00;
   }

   //dest[++i] = '\0';
   return dest[0];
}

char *deepcopy_char_array(char src[], char dest[])
{
   int i = 0, j = 0;
   while (src[j])
      dest[i++] = src[j++];
   while(dest[i]){
      dest[i++] = 0x00; //there can be extra chars
   }
   return dest;
}

//The cursor is already disabled via a BIOS function, but perhaps I'll switch to this method someday
void disable_cursor()
{
   outb(0x3D4, 0x0A);
   outb(0x3D5, 0x20);
}



//int to character
unsigned char *int_to_char(unsigned long value, unsigned char *str, unsigned int base)
{
  unsigned char *rc;
  unsigned char *ptr;
  unsigned char *low;
  // Check for supported base.
  if (base < 2 || base > 36)
  {
    *str = '\0';
    return str;
  }
  rc = ptr = str;
  // Set '-' for negative decimals.
  if (value < 0 && base == 10)
  {
    *ptr++ = '-';
  }
  // Remember where the numbers start.
  low = ptr;
  // The actual conversion.
  do
  {
    // Modulo is negative for negative value. This trick makes abs() unnecessary.
    *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
    value /= base;
  } while (value);
  // Terminating the string.
  *ptr-- = '\0';
  // Invert the numbers.
  while (low < ptr)
  {
    unsigned char tmp = *low;
    *low++ = *ptr;
    *ptr-- = tmp;
  }
  return rc;
}
