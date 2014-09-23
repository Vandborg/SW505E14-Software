#include "utility_sound.h"  
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"


// System sounds
void play_sound_startup(void)
{
	ecrobot_sound_tone(800, 300, 60);
	systick_wait_ms(150);
	ecrobot_sound_tone(1200, 150, 60);
	systick_wait_ms(150);
	ecrobot_sound_tone(2000, 150, 60);
	systick_wait_ms(200);
	ecrobot_sound_tone(2000, 150, 60);
}

void play_sound_shutdown(void)
{
	ecrobot_sound_tone(2000, 300, 60);
	systick_wait_ms(150);
	ecrobot_sound_tone(1200, 150, 60);
	systick_wait_ms(150);
	ecrobot_sound_tone(800, 150, 60);
	systick_wait_ms(200);
	ecrobot_sound_tone(800, 150, 60);
}

void play_sound_button_feedback(void)
{
	ecrobot_sound_tone(2000, 100, 60);
}

void play_sound_mode_wait(void)
{
	ecrobot_sound_tone(1700, 100, 60);
	systick_wait_ms(100);
	ecrobot_sound_tone(1800, 100, 60);
	systick_wait_ms(100);
	ecrobot_sound_tone(1700, 100, 100);
}

void play_sound_mode_select_menu(void)
{
	ecrobot_sound_tone(1600, 100, 60);
	systick_wait_ms(100);
	ecrobot_sound_tone(1700, 100, 60);
	systick_wait_ms(100);
	ecrobot_sound_tone(1800, 200, 100);
}

void play_sound_mode_selected_default(void)
{
	ecrobot_sound_tone(500, 300, 60);
	systick_wait_ms(500);
	ecrobot_sound_tone(500, 60, 60);
	systick_wait_ms(100);
	ecrobot_sound_tone(500, 60, 60);
	systick_wait_ms(100);
	ecrobot_sound_tone(500, 60, 60);
	systick_wait_ms(100);
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

