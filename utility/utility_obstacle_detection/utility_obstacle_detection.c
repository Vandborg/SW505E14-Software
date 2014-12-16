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
#include "utility/utility_sound/utility_sound.h"
#include "utility/utility_bluetooth/utility_bluetooth.h"

DeclareTask(TASK_obstacle_detection);

#define MAX_DISTANCE 255
#define MIN_DISTANCE 0

#define NUMBER_OF_INSTRUCTIONS_FOR_DELIVER 6
#define NUMBER_OF_INSTRUCTIONS_FOR_FETCH 5

int measurements[3] = {MAX_DISTANCE, MAX_DISTANCE, MAX_DISTANCE};
int current_index = 0;

TASK(TASK_obstacle_detection)
{
    bool front_sonar_sensor_active = false;
    
    switch(Navigation.type_of_task)
    {
        case TYPE_DELIVER_PALLET:
            front_sonar_sensor_active = 
                Navigation.next > NUMBER_OF_INSTRUCTIONS_FOR_DELIVER;
            break;
        case TYPE_FETCH_PALLET:
            front_sonar_sensor_active = 
                Navigation.next > NUMBER_OF_INSTRUCTIONS_FOR_FETCH;
            break;
        case TYPE_NAVIGATE_TO:
            if(Navigation.next > -1)
            {
                front_sonar_sensor_active = 
                    Navigation.directions[Navigation.next] != 'U' &&
                    Navigation.directions[Navigation.next] != 'D' &&
                    Navigation.directions[Navigation.next] != 'T';
            }
            else
            {
                front_sonar_sensor_active = false;
            }
            break;
        default:
            front_sonar_sensor_active = false;
            break;
    }

    if(front_sonar_sensor_active)
    {
        measurements[current_index] = 
            ecrobot_get_sonar_sensor(SONAR_SENSOR_FRONT);

        current_index = (current_index + 1) % 3;

        int distance = (measurements[0] +
                        measurements[1] +
                        measurements[2]) / 3;


        if(distance <= OBSTACLE_DISTANCE_THRESHOLD_FRONT)
        {
            emergency_stop();

            report_obstacle_bt();

            measurements[0] = MAX_DISTANCE;
            measurements[1] = MAX_DISTANCE;
            measurements[2] = MAX_DISTANCE;
        }
    }
    
    TerminateTask();
}


