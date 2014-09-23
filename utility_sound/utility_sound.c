#include "utility_sound.h"  
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"


// System sounds
void play_sound_startup(void)
{
	ecrobot_sound_tone(2000, 100, 60);
	systick_wait_ms(100);
	ecrobot_sound_tone(500, 100, 60);
	systick_wait_ms(100);
	ecrobot_sound_tone(2000, 100, 60);
}

void play_sound_button_feedback(void)
{
	ecrobot_sound_tone(2000, 100, 60);
}

void play_sound_end_task(void)
{
	ecrobot_sound_tone(500, 100, 60);
	systick_wait_ms(200);
	ecrobot_sound_tone(500, 100, 60);
	systick_wait_ms(200);
}

// Music
void play_sound_dsb(void)
{
	ecrobot_sound_tone(293, 600, 60);
	systick_wait_ms(650);
	ecrobot_sound_tone(311, 200, 60);
	systick_wait_ms(200);
	ecrobot_sound_tone(233, 800, 60);
	systick_wait_ms(800);
}

