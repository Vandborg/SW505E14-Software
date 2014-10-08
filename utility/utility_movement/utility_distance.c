// Third party libraries
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"

// Own libraries
#include "utility/utility_definitions/utility_definitions.h"

#define WHEEL_CIRCUMFERENCE_MM 134
#define MM_TO_CM 10
#define DEGREES_IN_CIRCLE 360

void reset_distance(void) 
{
    nxt_motor_set_count(LEFT_MOTOR, 0);
    nxt_motor_set_count(RIGHT_MOTOR, 0);
}

int current_distance(void)
{
    int distance_left = ((nxt_motor_get_count(LEFT_MOTOR) * 
                        WHEEL_CIRCUMFERENCE_MM) / 
                        DEGREES_IN_CIRCLE) / 
                        MM_TO_CM;

    int distance_right = ((nxt_motor_get_count(RIGHT_MOTOR) *
                         WHEEL_CIRCUMFERENCE_MM) / 
                         DEGREES_IN_CIRCLE) / 
                         MM_TO_CM;

    int distance_average = (distance_left + distance_right) / 2;

    return distance_average;
}
