#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "string.h"
#include <limits.h>

// Own header
#include "boot_programs/color_scanning.h"

// Own libraries
#include "utility_definitions/utility_definitions.h"
#include "utility_lcd/utility_lcd.h"
#include "utility_sound/utility_sound.h"

// Number of scans to be executed
#define NUMBER_OF_COLOR_SCANS 3000

// Scans and outputs values from the color scanner
void scan_colors(void)
{
	// Set the mode for the color sensor.
	ecrobot_set_nxtcolorsensor(COLOR_SENSOR, COLORSENSOR);

	// Prepare the display
	display_clear(1);
	lcd_display_line(0, "- COLOR SENSOR -", 0);
	lcd_display_line(1, "================", 0);
	lcd_display_line(3, "R: ", 0);
	lcd_display_line(4, "G: ", 0);
	lcd_display_line(5, "B: ", 0);
	lcd_display_line(6, "================", 0);
	lcd_display_line(7, "      EXIT  SCAN", 1);

	// Boolean to check if run button is pressed
	int run_button_press = 0;	

	// Integer to hold time of pressing buttons
	int time_on_first_enter_button_press = INT_MAX;

	// Loop until the user breaks the loop by holding the enter button
	while(1)
	{
		// Wait for the user to press the run button
		if(ecrobot_is_RUN_button_pressed() && !run_button_press)
		{
			// Set the run button to be pressed
			run_button_press = 1;

			// Tell the user that it is scanning
			lcd_display_line(2, "Scanning ...", 0);
			lcd_clear_line(3, 0);
			lcd_clear_line(4, 0);
			lcd_clear_line(5, 0);
			lcd_display_line(3, "R: ", 0);
			lcd_display_line(4, "G: ", 0);
			lcd_display_line(5, "B: ", 1);
			play_sound(SOUND_MODE_WAIT);
			
			// Read the color
			S16 rgb[3] = {0, 0, 0};

			int red = rgb[0];
			int green = rgb[1]; 
			int blue = rgb[2];

			// Read the color 100 times
			for(int i = 0; i < NUMBER_OF_COLOR_SCANS; i++)
			{
				ecrobot_get_nxtcolorsensor_rgb(COLOR_SENSOR, rgb);
				red += rgb[0];
				green += rgb[1];
				blue += rgb[2];
			}

			// Find avarage
			red = red / NUMBER_OF_COLOR_SCANS;
			green = green / NUMBER_OF_COLOR_SCANS;
			blue = blue / NUMBER_OF_COLOR_SCANS;

			lcd_clear_line(2, 1);
			display_goto_xy(3, 3);
			display_int(red, 3);
			display_goto_xy(3, 4);
			display_int(green, 3);
			display_goto_xy(3, 5);
			display_int(blue, 3);
			display_update();	
			play_sound(SOUND_NOTIFICATION);	
		}
		// Has the enter button been released
		else if(!ecrobot_is_RUN_button_pressed())
		{
			run_button_press = 0;
		}
		
		// Chek if the enter button is pressed for long enough time to exit
		if(ecrobot_is_ENTER_button_pressed())
		{	
			// Time when the button was pressed
			int enter_button_pressed_start_time = systick_get_ms();

			// Continusly check if enought time has past to exit
			while(ecrobot_is_ENTER_button_pressed())
			{
				if(enter_button_pressed_start_time + ENTER_BUTTON_EXIT_TIMEOUT < systick_get_ms()) 
				{	
					return;
				}
			}
		}
	}	
}
