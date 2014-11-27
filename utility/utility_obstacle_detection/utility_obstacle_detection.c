// Third party libraries
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"
#include "stdlib.h"
#include "string.h"

// Own header
#include "utility/utility_bluetooth/utility_bluetooth.h"

// Own libraries
#include "utility/utility_lcd/utility_lcd.h"
#include "utility/utility_variables/utility_variables.h"
#include "utility/utility_string/utility_string.h"
#include "utility/utility_structs/utility_structs.h"

// DeclareTask(TASK_detect_obstacle);

// S32 distance_left = 0;
// S32 distance_right = 0;
// int measurements = 0;

// // This is unfinished. Complete the task to make the object detection
// // functionality
// TASK(TASK_detect_obstacle)
// {
    
//     distance_left += ecrobot_get_sonar_sensor(SONAR_SENSOR_FRONT);
//     distance_right += ecrobot_get_sonar_sensor(SONAR_SENSOR_REAR);

//     // Calculate average of new and previous
//     distance_left /= 2;
//     distance_right /= 2;
//     TerminateTask();
// }



