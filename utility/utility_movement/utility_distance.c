// Third party libraries
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"

// Own libraries
#include "utility/utility_definitions/utility_definitions.h"

#define WHEEL_CIRCUMFERENCE_MM 134
#define MM_TO_CM 10
#define DEGREES_IN_CIRCLE 360

int start_motor_count_right = 0;
int start_motor_count_left = 0;

void reset_distance(void) 
{
    start_motor_count_right = nxt_motor_get_count(RIGHT_MOTOR);
    start_motor_count_left = nxt_motor_get_count(LEFT_MOTOR);
}

int current_distance(void)
{
    int distance_left = (nxt_motor_get_count(LEFT_MOTOR) -
                        start_motor_count_left) * 
                        WHEEL_CIRCUMFERENCE_MM / 
                        DEGREES_IN_CIRCLE / 
                        MM_TO_CM;

    int distance_right = (nxt_motor_get_count(RIGHT_MOTOR) - 
                         start_motor_count_right) *
                         WHEEL_CIRCUMFERENCE_MM / 
                         DEGREES_IN_CIRCLE / 
                         MM_TO_CM;

    int distance_average = (distance_left + distance_right) / 2;

    return distance_average;
}
