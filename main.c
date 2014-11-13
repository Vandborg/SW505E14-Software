// Third party libraries
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"

// Own libraries
#include "boot_programs/boot_programs.h"
#include "utility/utility_bluetooth/utility_bluetooth.h"
#include "utility/utility_definitions/utility_definitions.h"
#include "utility/utility_movement/utility_movement.h"
#include "utility/utility_sound/utility_sound.h"
#include "utility/utility_structs/utility_structs.h"

navigation Navigation;

DeclareCounter(SysTimerCnt);

// System clock
void user_1ms_isr_type2(void) 
{
    SignalCounter(SysTimerCnt);
}

// On device initialization
void ecrobot_device_initialize(void)
{
    ecrobot_init_nxtcolorsensor(COLOR_SENSOR_LEFT, NXT_COLORSENSOR);
    ecrobot_init_nxtcolorsensor(COLOR_SENSOR_RIGHT, NXT_LIGHTSENSOR_RED);
    ecrobot_init_sonar_sensor(SONAR_SENSOR_FRONT);
    ecrobot_init_sonar_sensor(SONAR_SENSOR_REAR);
    ecrobot_init_bt_slave("password");
}

// On device termination
void ecrobot_device_terminate(void)
{
    ecrobot_term_nxtcolorsensor(COLOR_SENSOR_LEFT);
    ecrobot_term_nxtcolorsensor(COLOR_SENSOR_RIGHT);
    ecrobot_term_sonar_sensor(SONAR_SENSOR_FRONT);
    ecrobot_term_sonar_sensor(SONAR_SENSOR_REAR);
}

void initialize_colors(void) 
{
    update_color_bt(COLOR_RED_LEFT);
    update_color_bt(COLOR_RED_RIGHT);
    update_color_bt(COLOR_BLUE_LEFT);
    update_color_bt(COLOR_BLUE_RIGHT);
    update_color_bt(COLOR_BLACK_LEFT);
    update_color_bt(COLOR_BLACK_RIGHT);
    update_color_bt(COLOR_GRAY_LEFT);
    update_color_bt(COLOR_GRAY_RIGHT);
    update_color_bt(COLOR_WHITE_LEFT);
    update_color_bt(COLOR_WHITE_RIGHT);
}

// The boot task of the program
TASK(TASK_boot) 
{   
    initialize_colors();
    boot_device();
    TerminateTask();
}


