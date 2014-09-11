#include "utility_lcd.h"  
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include <string.h>

/* Writes a string of chars on a given line on the display returns 1 if it succeeds */
int write_to_lcd_line(int line_number, char* string)
{
   char* output;
   // Check if the line number argument was valid
   if(line_number >= 0 && line_number < 8)
   {  
         // Strips the string down to 16 characters
         memcpy(output, string, 16);

         display_goto_xy(0, line_number);
         display_string(output);
         display_update();
         return 1;  
   }

   // The writing was unsuccesful
   return 0;
}
