// Third party libraries
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"

// Own header
#include "utility_line_follow.h"
#include "../utility_sound/utility_sound.h"

DeclareTask(TASK_line_follow);
DeclareTask(TASK_color_scan);
DeclareAlarm(cyclic_alarm);
DeclareAlarm(cyclic_alarm_2);


// Persistent variables for PID, and color_scan/swap
U8 color_sensor = COLOR_SENSOR_LEFT;
U8 light_sensor = COLOR_SENSOR_RIGHT;

int offset_left = (416+140)/2;
int offset_right = (513+220)/2;

int integral = 0;
int lastError = 0;

bool line_follow = true;

S16 rgb[3];

void start_line_following(void)
{
    GetResource(RES_SCHEDULER);

    ecrobot_set_nxtcolorsensor(color_sensor, NXT_COLORSENSOR);
    ecrobot_set_nxtcolorsensor(light_sensor, NXT_COLORSENSOR);
    ecrobot_process_bg_nxtcolorsensor();

    SetRelAlarm(cyclic_alarm, 1, 50);
    SetRelAlarm(cyclic_alarm_2, 1, 300);

    ReleaseResource(RES_SCHEDULER);
}

void stop_line_following(void)
{
    GetResource(RES_SCHEDULER);

    nxt_motor_set_speed(RIGHT_MOTOR, 0, 1);
    nxt_motor_set_speed(LEFT_MOTOR, 0, 1);

    CancelAlarm(cyclic_alarm);
    CancelAlarm(cyclic_alarm_2);
    
    ecrobot_set_nxtcolorsensor(color_sensor, NXT_LIGHTSENSOR_NONE);
    ecrobot_set_nxtcolorsensor(light_sensor, NXT_LIGHTSENSOR_NONE);
    ecrobot_process_bg_nxtcolorsensor();

    ReleaseResource(RES_SCHEDULER);
}



TASK(TASK_line_follow)
{   
    if (line_follow)
    {
        int powerA = 0;
        int powerB = 0;

        int error = 0;
        int derivative = 0;
        int turn = 0;

        ecrobot_process_bg_nxtcolorsensor();

        S16 rgb2[3];
        ecrobot_get_nxtcolorsensor_rgb(light_sensor, rgb2);

        int lightLevel = (rgb2[0] + rgb2[1] + rgb2[2]) / 3;//ecrobot_get_nxtcolorsensor_light(light_sensor);

        if (light_sensor == COLOR_SENSOR_LEFT)
        {
            error = lightLevel - offset_left; 
            integral = integral + error;
            derivative = error - lastError;

            turn = KP * error + KI * integral + KD * derivative;
            
            // This is needed because the k's are multiplied by a hundred
            turn = turn / 100; 

            powerA = LINE_FOLLOW_SPEED + turn;
            powerB = LINE_FOLLOW_SPEED - turn;
        }
        else
        {
            error = lightLevel - offset_right;
            integral = integral + error;
            derivative = error - lastError;
            
            turn = KP * error + KI * integral + KD * derivative;
            
            // This is needed because the k's are multiplied by a hundred
            turn = turn / 100; 

            powerA = LINE_FOLLOW_SPEED - turn;
            powerB = LINE_FOLLOW_SPEED + turn;    
        }

        nxt_motor_set_speed(LEFT_MOTOR, powerA, 1);
        nxt_motor_set_speed(RIGHT_MOTOR, powerB, 1);

        lastError = error;
    }
  

    TerminateTask();
}

bool last_color_red = false;

TASK(TASK_color_scan) // NEEDS REWORK
{
    // ecrobot_get_nxtcolorsensor_rgb(color_sensor, rgb);

    // // If read value is red
    // if (rgb[0] > 300 && 
    //     rgb[1] < 350 && 
    //     rgb[2] < 350 && 
    //     !last_color_red)
    // {
    //     U8 temp = color_sensor;
    //     color_sensor = light_sensor;
    //     light_sensor = temp;

    //     integral = 0;
    //     lastError = 0;
    //     // stop_line_following();
    //     last_color_red = true;
    // }
    // else 
    // {
    //     last_color_red = false;
    // }

    TerminateTask();
}
