#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "string.h"

// Own libraries
#include "utility_definitions/utility_definitions.h"
#include "utility_mode_handling/utility_mode_handling.h" 
#include "utility_sound/utility_sound.h"  
#include "color_scanning/color_scanning.h"
#include "sonar_sensor/sonar_sensor.h"

// Hook routines

// System clock
void user_1ms_isr_type2(void) { }

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

TASK(Task1) 
{ 
	check_startup_mode();

	TerminateTask();
}
