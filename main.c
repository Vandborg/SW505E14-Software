// Third party libraries
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"

// Own libraries
#include "boot_programs/boot_programs.h"
#include "utility/utility_definitions/utility_definitions.h"
#include "utility/utility_movement/utility_movement.h"
#include "utility/utility_variables/utility_variables.h"

// Global variables
color Color_left[AMOUNT_OF_COLORS];
color Color_right[AMOUNT_OF_COLORS];

// System clock
void user_1ms_isr_type2(void) 
{ 
}

// On device initialization
void ecrobot_device_initialize(void)
{
    // Initialize color values
    for(int i = 0; i < AMOUNT_OF_COLORS; i++)
    {
        Color_left[i].red = 0;
        Color_left[i].green = 0;
        Color_left[i].blue = 0;
        Color_right[i].red = 0;
        Color_right[i].green = 0;
        Color_right[i].blue = 0;
    }

	ecrobot_init_nxtcolorsensor(COLOR_SENSOR);
	ecrobot_init_sonar_sensor(SONAR_SENSOR);
}

// On device termination
void ecrobot_device_terminate(void)
{
	ecrobot_term_nxtcolorsensor(COLOR_SENSOR);
	ecrobot_term_sonar_sensor(SONAR_SENSOR);
}

// The boot task of the program
TASK(TASK_boot) 
{   
    boot_device();

	TerminateTask();
}
