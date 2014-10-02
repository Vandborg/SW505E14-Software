// Third party libraries
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"

// Own libraries
#include "../utility_definitions/utility_definitions.h"

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
