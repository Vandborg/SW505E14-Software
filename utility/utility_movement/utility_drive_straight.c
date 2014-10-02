// Third party libraries
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"

// Own libraries
#include "../utility_definitions/utility_definitions.h"
#include "utility_distance.h"

void drive_forward(int speed, int brake)
{
    if(speed == SLOW ||
       speed == MEDIUM ||
       speed == FAST)
    {
        int left_speed = speed;
        int right_speed = speed - 1;
        nxt_motor_set_speed(LEFT_MOTOR, left_speed, 1);
        nxt_motor_set_speed(RIGHT_MOTOR, right_speed, 1);
    }
}


void drive_forward_distance(int speed, int brake, int distance)
{
    reset_distance();
    int left_speed = speed;
    int right_speed = speed;

    while(current_distance() <= distance)
    {
        int right_motor_count = nxt_motor_get_count(RIGHT_MOTOR);
        int left_motor_count = nxt_motor_get_count(LEFT_MOTOR);

        if (right_motor_count > left_motor_count)
        {
            right_speed--;
            left_speed++;
        }
        else if(left_motor_count > right_motor_count)
        {
            right_speed++;
            left_speed--;
        }

        nxt_motor_set_speed(RIGHT_MOTOR, right_speed, brake);
        nxt_motor_set_speed(LEFT_MOTOR, left_speed, brake);
    }
}
