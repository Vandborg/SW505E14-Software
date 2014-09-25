#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include <string.h>

// Own header
#include "utility_lcd.h"

#define LCD_MIN_LINE 0
#define LCD_MAX_LINE 8
#define STR_MAX_LENGTH 16

// Writes a string of chars on a given line on the display returns 1 if it succeeds
int lcd_display_line(int line_number, char* string, int update_display)
{
	// Check if the line number argument was valid
	if(line_number >= LCD_MIN_LINE && line_number <= LCD_MAX_LINE && strlen(string) <= STR_MAX_LENGTH)
	{
		// Clear the line
		lcd_clear_line(line_number, 0);

		// Update the correct buffer line of the display
		display_goto_xy(0, line_number);
		display_string(string);

		// Check if the display should be updated
		if(update_display) 
		{
			display_update();
		}
	
		// Everything went well
		return 1;  
	}

	// The writing was unsuccesful
	return 0;
}

// Clears a line on the display
void lcd_clear_line(int line_number, int update_display)
{
	// Write empty space to the line in order to clear it
	display_goto_xy(0, line_number);
	display_string("                ");

	// Check if the display should be updated
	if(update_display) 
	{
		display_update();
	}
}
