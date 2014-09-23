#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "string.h"

// Own libraries
#include "utility_sound/utility_sound.h"  

#define LEFT_MOTOR NXT_PORT_A
#define RIGHT_MOTOR NXT_PORT_B
#define COLOR_SENSOR NXT_PORT_S1

#define NUMBER_OF_COLORS 2
#define NUMBER_OF_COLOR_SCANS 400

/* nxtOSEK hook to be invoked from an ISR in category 2 */
void user_1ms_isr_type2(void) { }

void ecrobot_device_initialize(void) {
	ecrobot_init_nxtcolorsensor(COLOR_SENSOR);
}

void ecrobot_device_terminate(void) {
	ecrobot_term_nxtcolorsensor(COLOR_SENSOR);
}

// Color struct
typedef struct {
	int red;
	int green;
	int blue;
	char name[10];
} color;

// Array of colors
color colors[NUMBER_OF_COLORS] = {{-1, -1, -1, "black"}, {-1, -1, -1, "white"}};

// Update a color in the array
void update_color(int index)
{
	play_sound_button_feedback();

	// Read the color
	S16 rgb[3] = {0, 0, 0};

	int red = rgb[0];
	int green = rgb[1]; 
	int blue = rgb[2];

	// Read the color 100 times
	for(int i = 0; i < NUMBER_OF_COLOR_SCANS; i++)
	{
		ecrobot_get_nxtcolorsensor_rgb(COLOR_SENSOR, rgb);
		red += rgb[0];
		green += rgb[1];
		blue += rgb[2];

		systick_wait_ms(5);
	}

	// Find avarage
	red = red / NUMBER_OF_COLOR_SCANS;
	green = green / NUMBER_OF_COLOR_SCANS;
	blue = blue / NUMBER_OF_COLOR_SCANS;

	// Update the color array
	color updated_color = {red, green, blue, "F"};
	colors[index] = updated_color;

	play_sound_end_task();
}

TASK(Task1) 
{ 
	play_sound_startup();

	ecrobot_set_nxtcolorsensor(COLOR_SENSOR, COLORSENSOR);

	while(1) 
	{
		for(int i = 0; i < NUMBER_OF_COLORS; i++)
		{
			while(!ecrobot_is_RUN_button_pressed())
			{
				// Busy wait
			}
			
			// Update the color on the sensor
			update_color(i);

			while(ecrobot_is_RUN_button_pressed())
			{
				// Busy wait
			}
		}

		// Display color array
		display_clear(0);
		display_goto_xy(0, 0);
		display_string("RGB OF COLORS");

		for(int j = 0; j < NUMBER_OF_COLORS; j++)
		{
			display_goto_xy(0, j+1);
			display_int(colors[j].red, 3);
			display_string(",");
			display_int(colors[j].green, 3);
			display_string(",");
			display_int(colors[j].blue, 3);
		}

		display_update();

	}

	TerminateTask();

}
