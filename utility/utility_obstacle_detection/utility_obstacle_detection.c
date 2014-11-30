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
#include "utility/utility_movement/utility_line_follow.h"
#include "utility/utility_movement/utility_car_rotate.h"
#include "utility/utility_sound/utility_sound.h"
#include "utility/utility_bluetooth/utility_bluetooth.h"

DeclareTask(TASK_obstacle_detection);

S32 distance_front = 255;
S32 distance_rear = 255;
bool driving_forward = true;

TASK(TASK_obstacle_detection)
{
    if(driving_forward)
    {
        distance_front += ecrobot_get_sonar_sensor(SONAR_SENSOR_FRONT);
        distance_front /= 2;

        if(distance_front <= OBSTACLE_DISTANCE_THRESHOLD_FRONT)
        {
            distance_front = 0;

            stop_line_following();

            turn_degrees(180);
        }
    }
    else
    {
        distance_rear += ecrobot_get_sonar_sensor(SONAR_SENSOR_REAR);   
        distance_rear /= 2;

        if(distance_rear <= OBSTACLE_DISTANCE_THRESHOLD_REAR)
        {
            distance_rear = 0;

            stop_line_following();

            turn_degrees(180);
        }
    }

    TerminateTask();
}


