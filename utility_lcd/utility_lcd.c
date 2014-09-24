#include "utility_lcd.h"  
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include <string.h>

// Writes a string of chars on a given line on the display returns 1 if it succeeds
int write_to_lcd_line(int line_number, char* string)
{
   // Check if the line number argument was valid
   if(line_number >= 0 && line_number < 8)
   {
         // Update the correct buffer line of the display
         display_goto_xy(0, line_number);
         display_string(string);
         
         // Clear the surplus characters of the line on the display
         display_string("                ");
         display_update();
         return 1;  
   }

   // The writing was unsuccesful
   return 0;
}

// Clears a line on the display
int clear_lcd_line(int line_number)
{
   write_to_lcd_line(line_number, "                ");
}
