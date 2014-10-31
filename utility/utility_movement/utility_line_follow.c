// Third party libraries
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"

// Own header
#include "utility_line_follow.h"

DeclareTask(TASK_line_follow);
DeclareTask(TASK_color_scan);
DeclareAlarm(cyclic_alarm);
DeclareAlarm(cyclic_alarm_2);


// Persistent variables for PID, and color_scan/swap
U8 color_sensor = COLOR_SENSOR_LEFT;
U8 light_sensor = COLOR_SENSOR_RIGHT;

int offset_left = 360;
int offset_right = 360;

int integral = 0;
int lastError = 0;

S16 rgb[3];

void start_line_following(void)
{
    GetResource(RES_SCHEDULER);

    ecrobot_set_nxtcolorsensor(color_sensor, NXT_COLORSENSOR);
    ecrobot_set_nxtcolorsensor(light_sensor, NXT_LIGHTSENSOR_RED);
    ecrobot_process_bg_nxtcolorsensor();

    SetRelAlarm(cyclic_alarm, 1, 50);
    SetRelAlarm(cyclic_alarm_2, 1, 300);

    ReleaseResource(RES_SCHEDULER);
}

void stop_line_following(void)
{
    GetResource(RES_SCHEDULER);

    CancelAlarm(cyclic_alarm);
    CancelAlarm(cyclic_alarm_2);
    
    ecrobot_set_nxtcolorsensor(color_sensor, NXT_LIGHTSENSOR_NONE);
    ecrobot_set_nxtcolorsensor(light_sensor, NXT_LIGHTSENSOR_NONE);
    ecrobot_process_bg_nxtcolorsensor();

    nxt_motor_set_speed(RIGHT_MOTOR, 0, 1);
    nxt_motor_set_speed(LEFT_MOTOR, 0, 1);

    ReleaseResource(RES_SCHEDULER);
}



TASK(TASK_line_follow)
{
    int powerA = 0;
    int powerB = 0;

    int error = 0;
    int derivative = 0;
    int turn = 0;

    ecrobot_process_bg_nxtcolorsensor();

    int lightLevel = ecrobot_get_nxtcolorsensor_light(light_sensor);

    if (light_sensor == COLOR_SENSOR_LEFT)
    {
        error = lightLevel - offset_left; 
        integral = integral + error;
        derivative = error - lastError;

        turn = KP_LEFT * error + KI_LEFT * integral + KD_LEFT * derivative;
        
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
        
        turn = KP_RIGHT * error + KI_RIGHT * integral + KD_RIGHT * derivative;
        
        // This is needed because the k's are multiplied by a hundred
        turn = turn / 100; 

        powerA = LINE_FOLLOW_SPEED - turn;
        powerB = LINE_FOLLOW_SPEED + turn;    
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

        integral = 0;
        lastError = 0;

        last_color_red = true;
    }
    else 
    {
        last_color_red = false;
    }

    TerminateTask();
}
