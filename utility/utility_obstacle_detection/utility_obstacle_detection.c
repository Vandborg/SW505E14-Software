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

#define MAX_DISTANCE 255
#define MIN_DISTANCE 0
#define HALF_ROTATION_DEGREES 180

S32 distance_front = MAX_DISTANCE;
S32 distance_rear = MAX_DISTANCE;
bool use_front_sonar_sensor;
bool use_rear_sonar_sensor;

TASK(TASK_obstacle_detection)
{
    
    if(use_front_sonar_sensor)
    {
        distance_rear = MAX_DISTANCE;
        distance_front += ecrobot_get_sonar_sensor(SONAR_SENSOR_FRONT);
        distance_front /= 2;

        if(distance_front <= OBSTACLE_DISTANCE_THRESHOLD_FRONT)
        {
            emergency_stop();

            turn_degrees(HALF_ROTATION_DEGREES);

            report_obstacle_bt();
        }
    }

    if(use_rear_sonar_sensor)
    {
        distance_front = MAX_DISTANCE;
        distance_rear += ecrobot_get_sonar_sensor(SONAR_SENSOR_REAR);   
        distance_rear /= 2;

        if(distance_rear <= OBSTACLE_DISTANCE_THRESHOLD_REAR)
        {
            emergency_stop();

            turn_degrees(HALF_ROTATION_DEGREES);

            report_obstacle_bt();
        }
    }

    TerminateTask();
}


