// Third party libraries
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"
#include "stdlib.h"
#include "string.h"

// Own header
#include "utility/utility_obstacle_detection/utility_obstacle_detection.h"

// Own libraries
#include "utility/utility_lcd/utility_lcd.h"
#include "utility/utility_variables/utility_variables.h"
#include "utility/utility_structs/utility_structs.h"
#include "utility/utility_sound/utility_sound.h"
#include "utility/utility_bluetooth/utility_bluetooth.h"

DeclareTask(TASK_obstacle_detection);

S32 distance_front = 0;
S32 distance_rear = 0;

TASK(TASK_obstacle_detection)
{
    distance_front += ecrobot_get_sonar_sensor(SONAR_SENSOR_FRONT);
    distance_rear += ecrobot_get_sonar_sensor(SONAR_SENSOR_REAR);

    distance_front /= 2;
    distance_rear /= 2;

    play_sound(SOUND_NOTIFICATION);

    if(distance_front >= OBSTACLE_DISTANCE_THRESHOLD)
    {
        nxt_motor_set_speed(LEFT_MOTOR, 0, 1);
        nxt_motor_set_speed(RIGHT_MOTOR, 0, 1);
        play_sound(SOUND_NOTIFICATION);
    }

    if(distance_rear >= OBSTACLE_DISTANCE_THRESHOLD)
    {
        nxt_motor_set_speed(LEFT_MOTOR, 0, 1);
        nxt_motor_set_speed(RIGHT_MOTOR, 0, 1);
        play_sound(SOUND_NOTIFICATION);
    }

    TerminateTask();
}


