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

	systick_wait_ms(2000);

	for(int i = 0; i < 4; i++)
	{
		if(!ecrobot_is_RUN_button_pressed())
		{
			play_sound(SOUND_MODE_WAIT);
			systick_wait_ms(1000);
		}
		else
		{
			select_mode();
			return;
		}
	}

	play_sound(SOUND_MODE_START);
}


// Used to select wanted mode
void select_mode(void)
{
	systick_wait_ms(1000);
	play_sound(SOUND_MODE_START);
	write_to_lcd_line(1, "SELECTING MODE");

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
			case 1 : write_to_lcd_line(2, "Default"); break;
			case 2 : write_to_lcd_line(2, "CountToTen"); break;
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
	write_to_lcd_line(1, "other call complete");
}
