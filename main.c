// Third party libraries
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"

// Own libraries
#include "boot_programs/boot_programs.h"
#include "utility/utility_definitions/utility_definitions.h"
#include "utility/utility_movement/utility_movement.h"
#include "utility/utility_sound/utility_sound.h"

DeclareCounter(SysTimerCnt);



// System clock
void user_1ms_isr_type2(void) 
{
    SignalCounter(SysTimerCnt);
}

// On device initialization
void ecrobot_device_initialize(void)
{
	ecrobot_init_nxtcolorsensor(COLOR_SENSOR_RIGHT, NXT_COLORSENSOR);
    ecrobot_init_nxtcolorsensor(COLOR_SENSOR_LEFT, NXT_COLORSENSOR);
	ecrobot_init_sonar_sensor(SONAR_SENSOR);
}

// On device termination
void ecrobot_device_terminate(void)
{
	ecrobot_term_nxtcolorsensor(COLOR_SENSOR_RIGHT);
    ecrobot_term_nxtcolorsensor(COLOR_SENSOR_LEFT);
	ecrobot_term_sonar_sensor(SONAR_SENSOR);
}

// The boot task of the program
TASK(TASK_boot) 
{   
    // boot_device();
    ecrobot_set_nxtcolorsensor(COLOR_SENSOR_LEFT, NXT_COLORSENSOR);
    ecrobot_set_nxtcolorsensor(COLOR_SENSOR_RIGHT, NXT_LIGHTSENSOR_RED);
    ecrobot_process_bg_nxtcolorsensor();

	TerminateTask();
}

S16 rgb[3];
int Tp = 60;

TASK(TASK_color_scan)
{
    ecrobot_get_nxtcolorsensor_rgb(COLOR_SENSOR_LEFT, rgb);

    if (rgb[0] > 300)
    {
        Tp = 40;
    }
    else 
    {
        Tp = 70;
    }

    TerminateTask();
}

int Kp = 28;
int Ki = 0.004;
int Kd = 19;
int offset = 330; 
int integral = 0;
int lastError = 0;

TASK(TASK_line_follow)
{
    int error = 0;
    int derivative = 0;
    int turn = 0;

    int powerA = 0;
    int powerB = 0;

    // LOGIC TO CHECK WHICH SENSOR TO USE <-----------------
    ecrobot_process_bg_nxtcolorsensor();
    int lightLevel = ecrobot_get_nxtcolorsensor_light(COLOR_SENSOR_RIGHT);

    //CALCULATE ERROR
    error = lightLevel - offset;
        
    integral = integral + error;

    derivative = error - lastError;

    turn = Kp * error + Ki * integral + Kd * derivative;
    turn = turn / 100; // ---> This is only needed if the k's 
                       //      are multiplied by a hundred
    powerA = Tp + turn;
    powerB = Tp - turn;

    // Make motors go... 
    nxt_motor_set_speed(LEFT_MOTOR, powerA, 1);
    nxt_motor_set_speed(RIGHT_MOTOR, powerB, 1);

    lastError = error;

    TerminateTask();
}

