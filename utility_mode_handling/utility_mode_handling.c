#include "utility_mode_handling.h"
#include "../utility_definitions/utility_definitions.h"
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include <string.h>
#include <stdio.h>

// Own libraries
#include "utility_sound/utility_sound.h"  
#include "utility_lcd/utility_lcd.h"  

// Number of modes including default
#define NUMBER_OF_MODES 2

// Used to check if a not default mode is wanted
void check_startup_mode(void)
{
	// Wait until the user releases the finger from the run button
	while(ecrobot_is_RUN_button_pressed()) {
		// Wait
	}

	int current_tick = systick_get_ms();
	int systick_target = current_tick + 5000;
	int next_tick = current_tick;

	// Give the user some time to interact with the unit
	while(current_tick <= systick_target) 
	{
		current_tick = systick_get_ms();

		// Check if the unit should tick
		if(next_tick <= current_tick) 
		{
			next_tick = next_tick + 1000;
			play_sound(SOUND_TICK);
		}

		// Check if the run button is pressed
		if(ecrobot_is_RUN_button_pressed()) {
			// Give button feedback
			play_sound(SOUND_BUTTON_FEEDBACK);

			// Go to the select mode menu
			select_mode();

			// exit from the loop
			break;
		}
	}

	// Return to the default program
	play_sound(SOUND_MODE_START);
}


// Used to select wanted mode
void select_mode(void)
{
	systick_wait_ms(1000);
	play_sound(SOUND_MODE_START);
	lcd_display_line(1, "SELECTING MODE", 1);

   int selected_mode = 1;
   int run_button_was_pressed = 0;
   while(1)
   {

		if(ecrobot_is_RUN_button_pressed())
		{
			run_button_was_pressed = 1;
		}
		else if(run_button_was_pressed && ecrobot_is_RUN_button_pressed() == 0)
		{
			selected_mode ++;
			if(selected_mode > NUMBER_OF_MODES)
			{
			selected_mode = 1;
			} 
		}
		
		switch(selected_mode)
		{
			case 1 : lcd_display_line(2, "Default", 1); break;
			case 2 : lcd_display_line(2, "CountToTen", 1); break;
		}
		
		if(0)
		{
			switch(selected_mode)
			{
				case 1 : return;
				case 2 : other_call();
			} 
		}

		 
	}
}

void other_call(void)
{
	display_clear(1);
	lcd_display_line(1, "other call complete", 1);
}
