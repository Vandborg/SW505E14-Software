// Third party libraries
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"

// Own header
#include "utility_sound.h"

// Own libraries
#include "utility/utility_definitions/utility_definitions.h"

// Sound levels
#define SOUND_LEVEL_LOW 	33
#define SOUND_LEVEL_MEDIUM 	66
#define SOUND_LEVEL_HIGH 	99

// Notes
#define NOTE_C_4 262
#define NOTE_D_4 294 
#define NOTE_E_4 330
#define NOTE_F_4 349
#define NOTE_G_4 392
#define NOTE_A_4 440
#define NOTE_B_4 494

#define NOTE_C_5 523
#define NOTE_D_5 587 
#define NOTE_E_5 659
#define NOTE_F_5 698
#define NOTE_G_5 784
#define NOTE_A_5 880
#define NOTE_B_5 987

#define NOTE_C_6 1046
#define NOTE_D_6 1174
#define NOTE_E_6 1318
#define NOTE_F_6 1396
#define NOTE_G_6 1567
#define NOTE_A_6 1760
#define NOTE_B_6 1975

// Note waits
#define NOTE_WAIT_WHOLE  1000
#define NOTE_WAIT_HALF   500
#define NOTE_WAIT_QUARTER 250
#define NOTE_WAIT_EIGHTH 125
#define NOTE_WAIT_SIXTEENTH 63
#define NOTE_WAIT_BETWEEN 25 // Time to wait between notes

void play_pause(int duration)
{
	systick_wait_ms(duration);
}

void play_note(int note, int duration, int sound_level, bool with_pause)
{
	ecrobot_sound_tone(note, duration, sound_level);
	if(with_pause)
	{
		play_pause(duration + NOTE_WAIT_BETWEEN);
	}
}

int play_sound(int sound) 
{
	// Select sound
	switch(sound) 
	{
		// Indicating that the unit has started
		case SOUND_STARTUP:
			play_note(NOTE_C_4, NOTE_WAIT_QUARTER, SOUND_LEVEL_LOW, true);
			play_note(NOTE_E_4, NOTE_WAIT_EIGHTH, SOUND_LEVEL_LOW, true);
			play_note(NOTE_G_4, NOTE_WAIT_EIGHTH, SOUND_LEVEL_LOW, true);
			play_pause(NOTE_WAIT_EIGHTH);
			play_note(NOTE_G_4, NOTE_WAIT_EIGHTH, SOUND_LEVEL_LOW, true);
			break;

		// Indicating that the unit is shutting down
		case SOUND_SHUTDOWN:
			play_note(NOTE_G_4, NOTE_WAIT_QUARTER, SOUND_LEVEL_LOW, true);
			play_note(NOTE_E_4, NOTE_WAIT_EIGHTH, SOUND_LEVEL_LOW, true);
			play_note(NOTE_C_4, NOTE_WAIT_EIGHTH, SOUND_LEVEL_LOW, true);
			play_pause(NOTE_WAIT_EIGHTH);
			play_note(NOTE_C_4, NOTE_WAIT_EIGHTH, SOUND_LEVEL_LOW, true);
			break;

		// Indicating that a button has been pressed
		case SOUND_BUTTON_FEEDBACK: 
			play_note(NOTE_B_6, NOTE_WAIT_EIGHTH, SOUND_LEVEL_LOW, true);
			break;

		// Indicating that the unit is waiting for something
		case SOUND_MODE_WAIT:
			play_note(NOTE_G_6, NOTE_WAIT_EIGHTH, SOUND_LEVEL_LOW, true);
			play_note(NOTE_F_6, NOTE_WAIT_EIGHTH, SOUND_LEVEL_MEDIUM, true);
			break;

		// Indicating that a mode has been started
		case SOUND_MODE_START:
			play_note(NOTE_D_5, NOTE_WAIT_QUARTER, SOUND_LEVEL_LOW, true);
			play_note(NOTE_G_5, NOTE_WAIT_EIGHTH, SOUND_LEVEL_LOW, true);
			play_note(NOTE_E_5, NOTE_WAIT_EIGHTH, SOUND_LEVEL_LOW, true);
			break;

		// Indicating that the unit created a notification
		case SOUND_NOTIFICATION:
			play_note(NOTE_F_4, NOTE_WAIT_EIGHTH, SOUND_LEVEL_LOW, true);
			play_note(NOTE_E_4, NOTE_WAIT_EIGHTH, SOUND_LEVEL_LOW, true);
			play_note(NOTE_F_4, NOTE_WAIT_EIGHTH, SOUND_LEVEL_LOW, true);
			break;

		case SOUND_TICK:
			play_note(NOTE_D_4, NOTE_WAIT_EIGHTH, SOUND_LEVEL_HIGH, false);
			break;

		default:
			return SOUND_INVALID_ID;
			break;
	}

	return SOUND_SUCCESS; // Everything went well
}
