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
	ecrobot_init_nxtcolorsensor(COLOR_SENSOR_LEFT, NXT_COLORSENSOR);
    ecrobot_init_nxtcolorsensor(COLOR_SENSOR_RIGHT, NXT_COLORSENSOR);
	ecrobot_init_sonar_sensor(SONAR_SENSOR);
}

// On device termination
void ecrobot_device_terminate(void)
{
	ecrobot_term_nxtcolorsensor(COLOR_SENSOR_LEFT);
    ecrobot_term_nxtcolorsensor(COLOR_SENSOR_RIGHT);
	ecrobot_term_sonar_sensor(SONAR_SENSOR);
}

int color_sensor = COLOR_SENSOR_LEFT;
int light_sensor = COLOR_SENSOR_RIGHT;

// The boot task of the program
TASK(TASK_boot) 
{   
    // boot_device();
    ecrobot_set_nxtcolorsensor(color_sensor, NXT_COLORSENSOR);
    ecrobot_set_nxtcolorsensor(light_sensor, NXT_LIGHTSENSOR_RED);
    ecrobot_process_bg_nxtcolorsensor();

    TerminateTask();
}

S16 rgb[3];
int Tp = 60;




int Kp = 27;
int Ki = 2;
int Kd = 30;
int offset = 360; 
int integral = 0;
int lastError = 0;

int error = 0;
int derivative = 0;
int turn = 0;

TASK(TASK_line_follow)
{
    error = 0;
    derivative = 0;
    turn = 0;

    int powerA = 0;
    int powerB = 0;

    // LOGIC TO CHECK WHICH SENSOR TO USE <-----------------
    ecrobot_process_bg_nxtcolorsensor();
    int lightLevel = ecrobot_get_nxtcolorsensor_light(light_sensor);

    //CALCULATE ERROR
    error = lightLevel - offset;
        
    integral = integral + error;

    derivative = error - lastError;

    turn = Kp * error + Ki * integral + Kd * derivative;
    turn = turn / 100; // ---> This is only needed if the k's 
                       //      are multiplied by a hundred

    if (light_sensor == COLOR_SENSOR_LEFT)
    {
        powerA = Tp + turn;
        powerB = Tp - turn;
    }
    else 
    {
        powerA = Tp - turn;
        powerB = Tp + turn;        
    }

    // Make motors go... 
    nxt_motor_set_speed(LEFT_MOTOR, powerA, 1);
    nxt_motor_set_speed(RIGHT_MOTOR, powerB, 1);

    lastError = error;

    TerminateTask();
}

int last_color_red = 0;

TASK(TASK_color_scan)
{
    ecrobot_get_nxtcolorsensor_rgb(color_sensor, rgb);

       
    if (rgb[0] > 300 && 
        rgb[1] < 350 && 
        rgb[2] < 350 && 
        !last_color_red)
    {
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

        int temp = color_sensor;
        color_sensor = light_sensor;
        light_sensor = temp;

        ecrobot_set_nxtcolorsensor(color_sensor, NXT_COLORSENSOR);
        ecrobot_set_nxtcolorsensor(light_sensor, NXT_LIGHTSENSOR_RED);
        ecrobot_process_bg_nxtcolorsensor();

        derivative = 0;
        integral = 0;
        lastError = 0;
        error = 0;

        last_color_red = 1;
        
    }
    else 
    {
        last_color_red = 0;
    }

    TerminateTask();
}
