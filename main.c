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
DeclareTask(TASK_line_follow);
DeclareTask(TASK_color_scan);
DeclareAlarm(cyclic_alarm);
DeclareAlarm(cyclic_alarm_2);



// System clock
void user_1ms_isr_type2(void) 
{
    SignalCounter(SysTimerCnt);
}

U8 color_sensor = COLOR_SENSOR_LEFT;
U8 light_sensor = COLOR_SENSOR_RIGHT;

// On device initialization
void ecrobot_device_initialize(void)
{
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

    if (false) // Set to true if line following should be activated
    {
        SetRelAlarm(cyclic_alarm, 1, 50);
        SetRelAlarm(cyclic_alarm_2, 1, 300);
    }

    TerminateTask();
}

// Persistent variables for PID, and color_scan/swap
int offset = 360;
int integral = 0;
int lastError = 0;
int error = 0;
int derivative = 0;
int turn = 0;
S16 rgb[3];

TASK(TASK_line_follow)
{
    int powerA = 0;
    int powerB = 0;

    error = 0;
    derivative = 0;
    turn = 0;

    ecrobot_process_bg_nxtcolorsensor();

    int lightLevel = ecrobot_get_nxtcolorsensor_light(light_sensor);

    error = lightLevel - offset; 
    integral = integral + error;
    derivative = error - lastError;

    turn = KP * error + KI * integral + KD * derivative;

    // This is needed because the k's are multiplied by a hundred
    turn = turn / 100; 

    // Changes the turn direction depending on which sensor is the lightsensor
    if (light_sensor == COLOR_SENSOR_LEFT)
    {
        powerA = TP + turn;
        powerB = TP - turn;
    }
    else 
    {
        powerA = TP - turn;
        powerB = TP + turn;        
    }

    nxt_motor_set_speed(LEFT_MOTOR, powerA, 1);
    nxt_motor_set_speed(RIGHT_MOTOR, powerB, 1);

    lastError = error;

    TerminateTask();
}

bool last_color_red = false;

TASK(TASK_color_scan) // NEEDS REWORK
{
    ecrobot_get_nxtcolorsensor_rgb(color_sensor, rgb);

    // If read value is red
    if (rgb[0] > 300 && 
        rgb[1] < 350 && 
        rgb[2] < 350 && 
        !last_color_red)
    {
        // Make device go closer to line 
        if(COLOR_SENSOR_LEFT == color_sensor)
        {
            nxt_motor_set_speed(RIGHT_MOTOR, 60, 1);
            nxt_motor_set_speed(LEFT_MOTOR, 0, 0);

            systick_wait_ms(750);

            nxt_motor_set_speed(RIGHT_MOTOR, 0, 0);
            nxt_motor_set_speed(LEFT_MOTOR, 60, 1);
        }
        else 
        {
            nxt_motor_set_speed(RIGHT_MOTOR, 0, 0);
            nxt_motor_set_speed(LEFT_MOTOR, 60, 1);

            systick_wait_ms(750);

            nxt_motor_set_speed(RIGHT_MOTOR, 60, 1);
            nxt_motor_set_speed(LEFT_MOTOR, 0, 0);
        }
        
        systick_wait_ms(750);

        nxt_motor_set_speed(RIGHT_MOTOR, 0, 1);
        nxt_motor_set_speed(LEFT_MOTOR, 0, 1);

        U8 temp = color_sensor;
        color_sensor = light_sensor;
        light_sensor = temp;

        ecrobot_set_nxtcolorsensor(color_sensor, NXT_COLORSENSOR);
        ecrobot_set_nxtcolorsensor(light_sensor, NXT_LIGHTSENSOR_RED);
        ecrobot_process_bg_nxtcolorsensor();

        derivative = 0;
        integral = 0;
        lastError = 0;
        error = 0;

        last_color_red = true;
    }
    else 
    {
        last_color_red = false;
    }

    TerminateTask();
}
