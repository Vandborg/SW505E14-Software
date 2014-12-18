// Third party libraries
#include "kernel.h"
#include "kernel_id.h"
#include "string.h"

// Own libraries
#include "utility/utility_string/utility_string.h"

// Source (reformatted): http://developer.mbed.org/forum/bugs-suggestions/topic/2319/
void reverse(s) char *s; 
{
    char *j;
    int c;
    j = s + strlen(s) - 1;

    while(s < j) 
    {
        c = *s;
        *s++ = *j;
        *j-- = c;
    }
}

// Source (reformatted): http://developer.mbed.org/forum/bugs-suggestions/topic/2319/
char* int_to_string(int number, char string[])
{
        int i, sign;

        // Record sign
        if ((sign = number) < 0)  
        {
            number = -number; // Make n positive 
        }

        i = 0; // Init counter

        // Generate digits in reverse order
        do 
        {       
            string[i++] = number % 10 + '0'; // get next digit */
        } 
        while ((number /= 10) > 0); // Delete it

        if (sign < 0)
        {
            string[i++] = '-';
        }
        
        string[i] = '\0';

        reverse(string);

        return string;
}

int char_to_int(const char c)
{
    switch (c)
    {
    case '0':
        return 0;
    case '1':
        return 1;
    case '2':
        return 2;
    case '3':
        return 3;
    case '4':
        return 4;
    case '5':
        return 5;
    case '6':
        return 6;
    case '7':
        return 7;
    case '8':
        return 8;
    case '9':
        return 9;
    default:
        return 0;
    }
}
