#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include <limits.h>

// Own header
#include "distance_measurement.h"

// Own libraries
#include "utility/utility_definitions/utility_definitions.h"
#include "utility/utility_lcd/utility_lcd.h"
#include "utility/utility_sound/utility_sound.h"

#define SONAR_SCAN_DELAY 200

// Scan and display the output of the sonar sensor
void distance_measurement(void)
{
	// Prepare the display
	display_clear(1);
	lcd_display_line(0, "- SONAR SENSOR -", 0);
	lcd_display_line(1, "================", 0);
	lcd_display_line(3, "CURRENT:", 0);
	lcd_display_line(4, "LOCK   :", 0);
	lcd_display_line(6, "================", 0);
	lcd_display_line(7, "      EXIT  LOCK", 1);

	// Boolean to check if run button is pressed
	int run_button_press = 0;	

	// The output from the sonar sensor
	int sensor_output = -1;

	// Loop until the user breaks the loop by holding the enter button
	while(1)
	{
		// Update the sonar output when its time to do so
		if(systick_get_ms() % SONAR_SCAN_DELAY == 0)
		{
			sensor_output = ecrobot_get_sonar_sensor(SONAR_SENSOR);
			display_goto_xy(9, 3);
			display_int(sensor_output, 3);
			display_update();
		}

		// Wait for the user to press the run button
		if(ecrobot_is_RUN_button_pressed() && !run_button_press)
		{
			// Set the run button to be pressed
			run_button_press = 1;

			display_goto_xy(9, 4);
			display_int(sensor_output, 3);
			display_update();
			play_sound(SOUND_BUTTON_FEEDBACK);
			
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
