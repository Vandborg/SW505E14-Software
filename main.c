// Third party libraries
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"
#include "string.h"

// Own libraries

#include "boot_programs/boot_programs.h"
#include "utility/utility_definitions/utility_definitions.h"
#include "utility/utility_sound/utility_sound.h"
#include "utility/utility_movement/utility_distance.h"
#include "utility/utility_movement/utility_drive_straight.h"
#include "utility/utility_movement/utility_braking.h"

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
    drive_forward(FAST,1);
    systick_wait_ms(5000);
    fullstop_brake();
	TerminateTask();
}
