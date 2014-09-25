#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "string.h"
#include <limits.h>

// Own header
#include "color_scanning/color_scanning.h"

// Own libraries
#include "utility_sound/utility_sound.h"
#include "utility_definitions/utility_definitions.h"

// Number of scans to be executed
#define NUMBER_OF_COLOR_SCANS 3000

// Scans and outputs values from the color scanner
void scan_colors(void)
{
	// Set the mode for the color sensor.
	ecrobot_set_nxtcolorsensor(COLOR_SENSOR, COLORSENSOR);

	// Prepare the display
	display_clear(1);
	display_goto_xy(0, 1);
	display_string("COLOR ON SENSOR:");
	display_goto_xy(0, 2);
	display_string("Press RUN-button");
	display_goto_xy(0, 3);
	display_string("to scan color.");
	display_update();

	// Boolean to check if run button is pressed
	int run_button_press = 0;	

	// Check if it is first time running pressing the RUN button
	int first_time_run_button_pressed = 1;

	// Integer to hold time of pressing buttons
	int time_on_first_enter_button_press = INT_MAX;

	// Loop until the user breaks the loop by pressing the holding the enter button
	while(1)
	{
		// Wait for the user to press the run button
		if(ecrobot_is_RUN_button_pressed() && !run_button_press)
		{
			// Set the run button to be pressed
			run_button_press = 1;

			// Clear the display on first scan.
			if(first_time_run_button_pressed)
			{
				display_clear(1);
				first_time_run_button_pressed = 0;
			}

			// Tell the user that it is scanning
			display_goto_xy(0, 1);
			display_string("COLOR ON SENSOR:");
			display_goto_xy(0, 2);
			display_string("Scanning ...");
			display_update();
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

			display_clear(1);
			display_goto_xy(0, 1);
			display_string("COLOR ON SENSOR:");
			display_goto_xy(0, 3);
			display_string("R: ");
			display_int(red, 3);
			display_goto_xy(0, 4);
			display_string("G: ");
			display_int(green, 3);
			display_goto_xy(0, 5);
			display_string("B: ");
			display_int(blue, 3);
			display_update();	

			play_sound(SOUND_NOTIFICATION);	
		}
		// Has the enter button been released
		else if(!ecrobot_is_RUN_button_pressed())
		{
			run_button_press = 0;
		}
		
		// Is the ENTER button not pressed
		if(!ecrobot_is_ENTER_button_pressed())
		{	
			// Reset the wait variable
			time_on_first_enter_button_press = systick_get_ms();
		}

		// Have the button been pressed long enough to leave the program
		if((systick_get_ms() - time_on_first_enter_button_press) > ENTER_BUTTON_EXIT_TIMEOUT)
		{
			return;
		}
	}	
}
