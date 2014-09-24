#include "utility_sound.h"
#include "../utility_definitions/utility_definitions.h"
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

void play_sound(int sound) 
{
	switch(sound) 
	{
		// Indicating that the unit has started
		case SOUND_STARTUP:
			ecrobot_sound_tone(800, 300, 60);
			systick_wait_ms(150);
			ecrobot_sound_tone(1200, 150, 60);
			systick_wait_ms(150);
			ecrobot_sound_tone(2000, 150, 60);
			systick_wait_ms(200);
			ecrobot_sound_tone(2000, 150, 60);
		break;

		// Indicating that the unit is shutting down
		case SOUND_SHUTDOWN:
			ecrobot_sound_tone(2000, 300, 60);
			systick_wait_ms(150);
			ecrobot_sound_tone(1200, 150, 60);
			systick_wait_ms(150);
			ecrobot_sound_tone(800, 150, 60);
			systick_wait_ms(200);
			ecrobot_sound_tone(800, 150, 60);
		break;

		// Indicating that a button has been pressed
		case SOUND_BUTTON_FEEDBACK: 
			ecrobot_sound_tone(2000, 100, 60);
		break;

		// Indicating that the unit is waiting for something
		case SOUND_MODE_WAIT:
			ecrobot_sound_tone(1700, 100, 60);
			systick_wait_ms(100);
			ecrobot_sound_tone(1800, 100, 60);
			systick_wait_ms(100);
			ecrobot_sound_tone(1700, 100, 100);
		break;

		// Indicating that a mode has been selected
		case SOUND_MODE_SELECT:
			ecrobot_sound_tone(1600, 100, 60);
			systick_wait_ms(100);
			ecrobot_sound_tone(1700, 100, 60);
			systick_wait_ms(100);
			ecrobot_sound_tone(1800, 200, 100);
		break;

		// Indicating that a mode has been started
		case SOUND_MODE_START:
			ecrobot_sound_tone(1600, 100, 60);
			systick_wait_ms(100);
			ecrobot_sound_tone(1700, 100, 60);
			systick_wait_ms(100);
			ecrobot_sound_tone(1800, 200, 100);
		break;

		// Indicating that the unit created a notification
		case SOUND_NOTIFICATION:
			ecrobot_sound_tone(500, 100, 60);
			systick_wait_ms(200);
			ecrobot_sound_tone(500, 100, 60);
			systick_wait_ms(200);
		break;
	}
}
