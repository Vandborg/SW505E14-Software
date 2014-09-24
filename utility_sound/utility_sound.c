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

void play_sound_mario(void)
{
		ecrobot_sound_tone(660, 100, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(660, 100, 100);
	systick_wait_ms(300);
	ecrobot_sound_tone(660, 100, 100);
	systick_wait_ms(300);
	ecrobot_sound_tone(510, 100, 100);
	systick_wait_ms(100);
	ecrobot_sound_tone(660, 100, 100);
	systick_wait_ms(300);
	ecrobot_sound_tone(770, 100, 100);
	systick_wait_ms(550);
	ecrobot_sound_tone(380, 100, 100);
	systick_wait_ms(575);

	ecrobot_sound_tone(510, 100, 100);
	systick_wait_ms(450);
	ecrobot_sound_tone(380, 100, 100);
	systick_wait_ms(400);
	ecrobot_sound_tone(320, 100, 100);
	systick_wait_ms(500);
	ecrobot_sound_tone(440, 100, 100);
	systick_wait_ms(300);
	ecrobot_sound_tone(480, 80, 100);
	systick_wait_ms(330);
	ecrobot_sound_tone(450, 100, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(430, 100, 100);
	systick_wait_ms(300);
	ecrobot_sound_tone(380, 100, 100);
	systick_wait_ms(200);
	ecrobot_sound_tone(660, 80, 100);
	systick_wait_ms(200);
	ecrobot_sound_tone(760, 50, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(860, 100, 100);
	systick_wait_ms(300);
	ecrobot_sound_tone(700, 80, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(760, 50, 100);
	systick_wait_ms(350);
	ecrobot_sound_tone(660, 80, 100);
	systick_wait_ms(300);
	ecrobot_sound_tone(520, 80, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(580, 80, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(480, 80, 100);
	systick_wait_ms(500);

	ecrobot_sound_tone(510, 100, 100);
	systick_wait_ms(450);
	ecrobot_sound_tone(380, 100, 100);
	systick_wait_ms(400);
	ecrobot_sound_tone(320, 100, 100);
	systick_wait_ms(500);
	ecrobot_sound_tone(440, 100, 100);
	systick_wait_ms(300);
	ecrobot_sound_tone(480, 80, 100);
	systick_wait_ms(330);
	ecrobot_sound_tone(450, 100, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(430, 100, 100);
	systick_wait_ms(300);
	ecrobot_sound_tone(380, 100, 100);
	systick_wait_ms(200);
	ecrobot_sound_tone(660, 80, 100);
	systick_wait_ms(200);
	ecrobot_sound_tone(760, 50, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(860, 100, 100);
	systick_wait_ms(300);
	ecrobot_sound_tone(700, 80, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(760, 50, 100);
	systick_wait_ms(350);
	ecrobot_sound_tone(660, 80, 100);
	systick_wait_ms(300);
	ecrobot_sound_tone(520, 80, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(580, 80, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(480, 80, 100);
	systick_wait_ms(500);

	ecrobot_sound_tone(500, 100, 100);
	systick_wait_ms(300);

	ecrobot_sound_tone(760, 100, 100);
	systick_wait_ms(100);
	ecrobot_sound_tone(720, 100, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(680, 100, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(620, 150, 100);
	systick_wait_ms(300);

	ecrobot_sound_tone(650, 150, 100);
	systick_wait_ms(300);
	ecrobot_sound_tone(380, 100, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(430, 100, 100);
	systick_wait_ms(150);

	ecrobot_sound_tone(500, 100, 100);
	systick_wait_ms(300);
	ecrobot_sound_tone(430, 100, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(500, 100, 100);
	systick_wait_ms(100);
	ecrobot_sound_tone(570, 100, 100);
	systick_wait_ms(220);

	ecrobot_sound_tone(500, 100, 100);
	systick_wait_ms(300);

	ecrobot_sound_tone(760, 100, 100);
	systick_wait_ms(100);
	ecrobot_sound_tone(720, 100, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(680, 100, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(620, 150, 100);
	systick_wait_ms(300);

	ecrobot_sound_tone(650, 200, 100);
	systick_wait_ms(300);

	ecrobot_sound_tone(1020, 80, 100);
	systick_wait_ms(300);
	ecrobot_sound_tone(1020, 80, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(1020, 80, 100);
	systick_wait_ms(300);

	ecrobot_sound_tone(380, 100, 100);
	systick_wait_ms(300);
	ecrobot_sound_tone(500, 100, 100);
	systick_wait_ms(300);

	ecrobot_sound_tone(760, 100, 100);
	systick_wait_ms(100);
	ecrobot_sound_tone(720, 100, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(680, 100, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(620, 150, 100);
	systick_wait_ms(300);

	ecrobot_sound_tone(650, 150, 100);
	systick_wait_ms(300);
	ecrobot_sound_tone(380, 100, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(430, 100, 100);
	systick_wait_ms(150);

	ecrobot_sound_tone(500, 100, 100);
	systick_wait_ms(300);
	ecrobot_sound_tone(430, 100, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(500, 100, 100);
	systick_wait_ms(100);
	ecrobot_sound_tone(570, 100, 100);
	systick_wait_ms(420);

	ecrobot_sound_tone(585, 100, 100);
	systick_wait_ms(450);

	ecrobot_sound_tone(550, 100, 100);
	systick_wait_ms(420);

	ecrobot_sound_tone(500, 100, 100);
	systick_wait_ms(360);

	ecrobot_sound_tone(380, 100, 100);
	systick_wait_ms(300);
	ecrobot_sound_tone(500, 100, 100);
	systick_wait_ms(300);
	ecrobot_sound_tone(500, 100, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(500, 100, 100);
	systick_wait_ms(300);

	ecrobot_sound_tone(500, 100, 100);
	systick_wait_ms(300);

	ecrobot_sound_tone(760, 100, 100);
	systick_wait_ms(100);
	ecrobot_sound_tone(720, 100, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(680, 100, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(620, 150, 100);
	systick_wait_ms(300);

	ecrobot_sound_tone(650, 150, 100);
	systick_wait_ms(300);
	ecrobot_sound_tone(380, 100, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(430, 100, 100);
	systick_wait_ms(150);

	ecrobot_sound_tone(500, 100, 100);
	systick_wait_ms(300);
	ecrobot_sound_tone(430, 100, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(500, 100, 100);
	systick_wait_ms(100);
	ecrobot_sound_tone(570, 100, 100);
	systick_wait_ms(220);

	ecrobot_sound_tone(500, 100, 100);
	systick_wait_ms(300);

	ecrobot_sound_tone(760, 100, 100);
	systick_wait_ms(100);
	ecrobot_sound_tone(720, 100, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(680, 100, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(620, 150, 100);
	systick_wait_ms(300);

	ecrobot_sound_tone(650, 200, 100);
	systick_wait_ms(300);

	ecrobot_sound_tone(1020, 80, 100);
	systick_wait_ms(300);
	ecrobot_sound_tone(1020, 80, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(1020, 80, 100);
	systick_wait_ms(300);

	ecrobot_sound_tone(380, 100, 100);
	systick_wait_ms(300);
	ecrobot_sound_tone(500, 100, 100);
	systick_wait_ms(300);

	ecrobot_sound_tone(760, 100, 100);
	systick_wait_ms(100);
	ecrobot_sound_tone(720, 100, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(680, 100, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(620, 150, 100);
	systick_wait_ms(300);

	ecrobot_sound_tone(650, 150, 100);
	systick_wait_ms(300);
	ecrobot_sound_tone(380, 100, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(430, 100, 100);
	systick_wait_ms(150);

	ecrobot_sound_tone(500, 100, 100);
	systick_wait_ms(300);
	ecrobot_sound_tone(430, 100, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(500, 100, 100);
	systick_wait_ms(100);
	ecrobot_sound_tone(570, 100, 100);
	systick_wait_ms(420);

	ecrobot_sound_tone(585, 100, 100);
	systick_wait_ms(450);

	ecrobot_sound_tone(550, 100, 100);
	systick_wait_ms(420);

	ecrobot_sound_tone(500, 100, 100);
	systick_wait_ms(360);

	ecrobot_sound_tone(380, 100, 100);
	systick_wait_ms(300);
	ecrobot_sound_tone(500, 100, 100);
	systick_wait_ms(300);
	ecrobot_sound_tone(500, 100, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(500, 100, 100);
	systick_wait_ms(300);

	ecrobot_sound_tone(500, 60, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(500, 80, 100);
	systick_wait_ms(300);
	ecrobot_sound_tone(500, 60, 100);
	systick_wait_ms(350);
	ecrobot_sound_tone(500, 80, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(580, 80, 100);
	systick_wait_ms(350);
	ecrobot_sound_tone(660, 80, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(500, 80, 100);
	systick_wait_ms(300);
	ecrobot_sound_tone(430, 80, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(380, 80, 100);
	systick_wait_ms(600);

	ecrobot_sound_tone(500, 60, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(500, 80, 100);
	systick_wait_ms(300);
	ecrobot_sound_tone(500, 60, 100);
	systick_wait_ms(350);
	ecrobot_sound_tone(500, 80, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(580, 80, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(660, 80, 100);
	systick_wait_ms(550);

	ecrobot_sound_tone(870, 80, 100);
	systick_wait_ms(325);
	ecrobot_sound_tone(760, 80, 100);
	systick_wait_ms(600);

	ecrobot_sound_tone(500, 60, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(500, 80, 100);
	systick_wait_ms(300);
	ecrobot_sound_tone(500, 60, 100);
	systick_wait_ms(350);
	ecrobot_sound_tone(500, 80, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(580, 80, 100);
	systick_wait_ms(350);
	ecrobot_sound_tone(660, 80, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(500, 80, 100);
	systick_wait_ms(300);
	ecrobot_sound_tone(430, 80, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(380, 80, 100);
	systick_wait_ms(600);

	ecrobot_sound_tone(660, 100, 100);
	systick_wait_ms(150);
	ecrobot_sound_tone(660, 100, 100);
	systick_wait_ms(300);
	ecrobot_sound_tone(660, 100, 100);
	systick_wait_ms(300);
	ecrobot_sound_tone(510, 100, 100);
	systick_wait_ms(100);
	ecrobot_sound_tone(660, 100, 100);
	systick_wait_ms(300);
	ecrobot_sound_tone(770, 100, 100);
	systick_wait_ms(550);
	ecrobot_sound_tone(380, 100, 100);
	systick_wait_ms(575);
}

