// Third party libraries
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"
#include "string.h"

// Own header
#include "boot_programs/color_measurement/color_measurement.h"

// Own libraries
#include "utility/utility_definitions/utility_definitions.h"
#include "utility/utility_lcd/utility_lcd.h"
#include "utility/utility_sound/utility_sound.h"

// Number of scans to be executed
#define NUMBER_OF_COLOR_SCANS 3000

void color_measurement(void)
{
	// Set the mode for the color sensor.
	ecrobot_set_nxtcolorsensor(COLOR_SENSOR, COLORSENSOR);

	// Prepare the display
	display_clear(TRUE);
	lcd_display_line(LCD_LINE_ONE,   "  MEASURE COLOR ", FALSE);
	lcd_display_line(LCD_LINE_TWO,   "================", FALSE);
	lcd_display_line(LCD_LINE_FOUR,  "R:              ", FALSE);
	lcd_display_line(LCD_LINE_FIVE,  "G:              ", FALSE);
	lcd_display_line(LCD_LINE_SIX,   "B:              ", FALSE);
	lcd_display_line(LCD_LINE_SEVEN, "================", FALSE);
	lcd_display_line(LCD_LINE_EIGHT, "      EXIT  SCAN", TRUE);

	// Boolean to check if run button is released
	int run_button_released = TRUE;

	// Loop until the user breaks the loop by holding the enter button
	while(TRUE)
	{
		// Wait for the user to press the run button
		if(ecrobot_is_RUN_button_pressed() && run_button_released)
		{
			run_button_released = FALSE; // The button is pressed

			// Tell the user that it is scanning
			lcd_display_line(LCD_LINE_THREE, "Scanning ...    ", FALSE);
			lcd_display_line(LCD_LINE_FOUR,  "R:              ", FALSE);
			lcd_display_line(LCD_LINE_FIVE,  "G:              ", FALSE);
			lcd_display_line(LCD_LINE_SIX,   "B:              ", TRUE);
			
			play_sound(SOUND_MODE_WAIT); // Play wait sound
			
			S16 rgb[3] = {0, 0, 0}; // Allocate space for color measurement

			// Used to calculate the average of color values.
			int red = rgb[0];
			int green = rgb[1]; 
			int blue = rgb[2];

			// Measure the color 
			for(int i = 0; i < NUMBER_OF_COLOR_SCANS; i++)
			{
				// Get the color from the sensor
				ecrobot_get_nxtcolorsensor_rgb(COLOR_SENSOR, rgb);

				// Increment the color values
				red += rgb[0];
				green += rgb[1];
				blue += rgb[2];
			}

			// Find avarage
			red = red / NUMBER_OF_COLOR_SCANS;
			green = green / NUMBER_OF_COLOR_SCANS;
			blue = blue / NUMBER_OF_COLOR_SCANS;

			// Update display with average color values
			lcd_clear_line(LCD_LINE_THREE, TRUE);
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
		else if(ecrobot_is_RUN_button_pressed() == FALSE)
		{
			run_button_released = TRUE;
		}
		
		// Chek if the enter button is pressed for long enough time to exit
		if(ecrobot_is_ENTER_button_pressed())
		{	
			// Time when the button was pressed
			int enter_button_pressed_start_time = systick_get_ms();

			// Continusly check if enought time has past to exit
			while(ecrobot_is_ENTER_button_pressed())
			{
				if(enter_button_pressed_start_time + ENTER_BUTTON_EXIT_TIMEOUT < 
				   systick_get_ms()) 
				{	
					return;
				}
			}
		}
	}	
}
