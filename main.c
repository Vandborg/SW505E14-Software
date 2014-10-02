// Third party libraries
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"
#include "string.h"

// Own libraries
#include "color_scanning/color_scanning.h"
#include "sonar_sensor/sonar_sensor.h"
#include "utility/utility_definitions/utility_definitions.h"
#include "utility/utility_mode_handling/utility_mode_handling.h" 
#include "utility/utility_sound/utility_sound.h"
#include "utility/utility_movement/utility_distance.h"

// System clock
void user_1ms_isr_type2(void) 
{ 
}

// On device initialization
void ecrobot_device_initialize(void)
{
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
	check_startup_mode();

	TerminateTask();
}
