// Third party libraries
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"

// Own libraries
#include "boot_programs/boot_programs.h"
#include "utility/utility_definitions/utility_definitions.h"
#include "utility/utility_movement/utility_movement.h"
#include "utility/utility_variables/utility_variables.h"
#include "utility/utility_sound/utility_sound.h"

DeclareCounter(SysTimerCnt);

// Global variables
color Color_left[AMOUNT_OF_COLORS];
color Color_right[AMOUNT_OF_COLORS];

// System clock
void user_1ms_isr_type2(void) 
{
    SignalCounter(SysTimerCnt);
}


// On device initialization
void ecrobot_device_initialize(void)
{
    // Initialize color values
    Color_left[COLOR_RED].red = 315;
    Color_left[COLOR_RED].green = 115;
    Color_left[COLOR_RED].blue = 117;
    Color_right[COLOR_RED].red = 365;
    Color_right[COLOR_RED].green = 200;
    Color_right[COLOR_RED].blue = 178;

    Color_left[COLOR_BLUE].red = 175;
    Color_left[COLOR_BLUE].green = 138;
    Color_left[COLOR_BLUE].blue = 172;
    Color_right[COLOR_BLUE].red = 207;
    Color_right[COLOR_BLUE].green = 219;
    Color_right[COLOR_BLUE].blue = 233;

    Color_left[COLOR_BLACK].red = 173;
    Color_left[COLOR_BLACK].green = 128;
    Color_left[COLOR_BLACK].blue = 125;
    Color_right[COLOR_BLACK].red = 200;
    Color_right[COLOR_BLACK].green = 195;
    Color_right[COLOR_BLACK].blue = 163;

    Color_left[COLOR_GRAY].red = 323;
    Color_left[COLOR_GRAY].green = 255;
    Color_left[COLOR_GRAY].blue = 258;
    Color_right[COLOR_GRAY].red = 418;
    Color_right[COLOR_GRAY].green = 398;
    Color_right[COLOR_GRAY].blue = 352;

    Color_left[COLOR_WHITE].red = 427;
    Color_left[COLOR_WHITE].green = 458;
    Color_left[COLOR_WHITE].blue = 351;
    Color_right[COLOR_WHITE].red = 500;
    Color_right[COLOR_WHITE].green = 488;
    Color_right[COLOR_WHITE].blue = 437;

    
    ecrobot_init_nxtcolorsensor(COLOR_SENSOR_LEFT, NXT_COLORSENSOR);
    ecrobot_init_nxtcolorsensor(COLOR_SENSOR_RIGHT, NXT_LIGHTSENSOR_RED);
    ecrobot_init_sonar_sensor(SONAR_SENSOR);
}

// On device termination
void ecrobot_device_terminate(void)
{
    ecrobot_term_nxtcolorsensor(COLOR_SENSOR_LEFT);
    ecrobot_term_nxtcolorsensor(COLOR_SENSOR_RIGHT);
    ecrobot_term_sonar_sensor(SONAR_SENSOR);
}



// The boot task of the program
TASK(TASK_boot) 
{   
    boot_device();
    // systick_wait_ms(500);
    start_line_following();
    
    TerminateTask();
}


