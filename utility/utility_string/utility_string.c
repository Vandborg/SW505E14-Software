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

    if ((sign = n) < 0)  /* record sign */
     n = -n;          /* make n positive */
    i = 0;
    do {       /* generate digits in reverse order */
     s[i++] = n % 10 + '0';   /* get next digit */
    } while ((n /= 10) > 0);     /* delete it */
    if (sign < 0)
     s[i++] = '-';
    s[i] = '\0';

    reverse(s);

    return s;
}
