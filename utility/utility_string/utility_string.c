// Third party libraries
#include "kernel.h"
#include "kernel_id.h"
#include "string.h"

// Own libraries
#include "../utility_string/utility_string.h"

// Stolen from http://developer.mbed.org/forum/bugs-suggestions/topic/2319/
void reverse(s) char *s; {
char *j;
int c;
 
  j = s + strlen(s) - 1;
  while(s < j) {
    c = *s;
    *s++ = *j;
    *j-- = c;
  }
}

// Stolen from http://developer.mbed.org/forum/bugs-suggestions/topic/2319/
char* int_to_string(int number, char string[])
{
    int i, sign;

    if ((sign = number) < 0)  /* record sign */
     number = -number;          /* make n positive */
    i = 0;
    do {       /* generate digits in reverse order */
     string[i++] = number % 10 + '0';   /* get next digit */
    } while ((number /= 10) > 0);     /* delete it */
    if (sign < 0)
     string[i++] = '-';
    string[i] = '\0';

    reverse(string);

    return string;
}
