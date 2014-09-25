#include "utility_mode_handling.h"
#include "../utility_definitions/utility_definitions.h"
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "string.h"
#include "stdio.h"

// Own libraries
#include "utility_sound/utility_sound.h"  
#include "utility_lcd/utility_lcd.h"
#include "color_scanning/color_scanning.h"
#include "sonar_sensor/sonar_sensor.h"

// Number of modes including default
#define NUMBER_OF_MODES 2

typedef struct {
	char* name;
	void (*func)(void);
} utility_mode_function;

// An array with space for ten startup programs
utility_mode_function functions[10];
int function_counter = 0;

// Used to add a function to the functions array
void add_utility_mode_function(char* name, void (*func)(void)) {
	// Create struct based on arguments
	utility_mode_function new_element = { .name = name, .func = func };

	// Add the new element to the array
	functions[function_counter] = new_element;

	// Increment the function counter
	function_counter++;
}

// Used to check if a not default mode is wanted
void check_startup_mode(void)
{
	// Clear the display
	display_clear(1);

	// Add all utility mode functions
	add_utility_mode_function("COLOR SENSOR", &scan_colors);
	add_utility_mode_function("SONAR SENSOR", &sonar_scan);

	// Wait until the user releases the finger from the run button
	while(ecrobot_is_RUN_button_pressed()) {
		// Wait
	}

	// Show boot menu
	lcd_display_line(0, "----- BOOT -----", 0);
	lcd_display_line(1, "================", 0);

	lcd_display_line(4, "[              ]", 0);

	lcd_display_line(6, "================", 0);
	lcd_display_line(7, "      STRT  MODE", 1);

	int current_tick = systick_get_ms();
	int next_tick = current_tick;
	int tick_delay = 500;

	int current_ticks = 0;
	int target_ticks = 14;

	// Give the user some time to interact with the unit
	while(current_ticks <= target_ticks) 
	{
		// Update the current tick
		current_tick = systick_get_ms();

		// Check if the unit should tick
		if(next_tick <= current_tick) 
		{
			// Play tick sound every second tick
			if(current_ticks % 2 == 0) 
			{
				play_sound(SOUND_TICK);
			}

			// Update loading bar
			display_goto_xy(current_ticks + 1, 4);
			display_string("#");
			display_update();

			// Set next tick
			next_tick = next_tick + tick_delay;
			current_ticks++;
		}

		// Check if the run button is pressed
		if(ecrobot_is_RUN_button_pressed()) {
			// Clear the display
			display_clear(1);

			// Go to the select mode menu
			select_mode();

			// Exit from the loop
			break;
		}

		if(ecrobot_is_ENTER_button_pressed()) {
			// Exit from the loop
			break;
		}
	}

	// Give button feedback
	play_sound(SOUND_BUTTON_FEEDBACK);

	// Clear the display
	display_clear(1);

	// Return to the default program
	play_sound(SOUND_MODE_START);
}

// Used to select wanted mode
void select_mode(void)
{
	// Wait until the user releases the finger from either the run or enter button
	while(ecrobot_is_RUN_button_pressed() || ecrobot_is_ENTER_button_pressed()) {
		// Wait
	}

	// Check if there are any functions to be used
	if(function_counter == 0) 
	{
		return;
	}

	// Used to check if the user has released the buttons before checking buttons again
	int run_button_pressed = 0;
	int enter_button_pressed = 0;

	// Indicates which mode is currently being displayed to the user
	// Initialised to be -1, which will simulate the first button press and update the display
	int mode = -1;

	// Add an additional mode that is used to exit the menu
	int exit_mode = 0;

	// Line number to display the selector item on
	int line = 3;

	// Loop until the user breaks
	while(1) 
	{
		// Check if the run button is pressed
		if((ecrobot_is_RUN_button_pressed() && !run_button_pressed) || mode == -1) 
		{
			// Set the menu
			if(mode == -1) {
				lcd_display_line(0, "----- MENU -----", 0);
				lcd_display_line(1, "================", 0);
				lcd_display_line(6, "================", 0);
				lcd_display_line(7, "  /   SLCT  NEXT", 1);
			}

			// Play button click sound
			play_sound(SOUND_BUTTON_FEEDBACK);

			// The button was pressed
			run_button_pressed = 1;

			// Increment mode correctly
			mode = (mode + 1) % (function_counter + 1);

			display_goto_xy(0, 7);
			display_int(mode, 2);
			display_goto_xy(3, 7);
			display_int(function_counter, 2);
			display_update();

			// Update the display
			if(mode == exit_mode) {
				lcd_display_line(line, "<EXIT>", 1);
			}
			else {
				lcd_display_line(line, functions[mode - 1].name, 1);
			}
		}
		else if(!ecrobot_is_RUN_button_pressed()) 
		{
			// The button was released
			run_button_pressed = 0;
		}

		// Check if the enter button is pressed
		if(ecrobot_is_ENTER_button_pressed() && !enter_button_pressed) 
		{
			// Button is pressed
			enter_button_pressed = 1;

			// Play button click sound
			play_sound(SOUND_BUTTON_FEEDBACK);

			// Wait until the user releases the button
			while(ecrobot_is_ENTER_button_pressed()) {
				// Busy wait
			}

			// Clear the display
			display_clear(1);

			// Test if the program is in the exit mode, and exit if it is
			if(mode == exit_mode) {
				break;
			}
			else {
				// Clear the display
				display_clear(1);

				// Call the function
				functions[mode - 1].func();

				// Reset the mode 
				mode = -1;

				// Clear the display
				display_clear(1);
			}
		}
		else if(!ecrobot_is_ENTER_button_pressed()) 
		{
			enter_button_pressed = 0;
		}
	}
}



void other_call(void)
{
	display_clear(1);
	lcd_display_line(1, "other call complete", 1);
}
