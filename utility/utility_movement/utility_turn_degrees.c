// Third party libraries
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"

// Own header
#include "utility_turn_degrees.h"

#define TURN_SPEED 60
#define MOTOR_COUNT_PER_DEGREE 3
#define MOTOR_INSECURITY 30
#define DEGREES_IN_CIRCLE 360

void turn_degrees(int degrees_to_turn) 
{
    bool counter_clockwise = false; 

    if(degrees_to_turn < 0)
    {
        degrees_to_turn = -degrees_to_turn;
        counter_clockwise = true;
    }

    int degrees_on_wheel = degrees_to_turn * MOTOR_COUNT_PER_DEGREE;

    int start_motor_count_left = nxt_motor_get_count(LEFT_MOTOR);
    int start_motor_count_right = nxt_motor_get_count(RIGHT_MOTOR);

    int left_motor_degrees = degrees_on_wheel - MOTOR_INSECURITY;
    int right_motor_degrees = -degrees_on_wheel;

    if(counter_clockwise)
    {
        nxt_motor_set_speed(LEFT_MOTOR, -TURN_SPEED, true);
        nxt_motor_set_speed(RIGHT_MOTOR, TURN_SPEED, true);
        
        left_motor_degrees = -left_motor_degrees;
        right_motor_degrees = -right_motor_degrees;
    }
    else 
    {
        nxt_motor_set_speed(LEFT_MOTOR, TURN_SPEED, true);
        nxt_motor_set_speed(RIGHT_MOTOR, -TURN_SPEED, true);
    }

    bool motor_one_running = true;
    bool motor_two_running = true;

    int rm_count = 0;
    int lm_count = 0;

    bool lm_count_cond = false;
    bool rm_count_cond = false;

    while(motor_one_running || motor_two_running)
    {   

        lm_count = nxt_motor_get_count(LEFT_MOTOR) - start_motor_count_left;
        rm_count = nxt_motor_get_count(RIGHT_MOTOR) - start_motor_count_right;

        lm_count_cond = lm_count >= left_motor_degrees;
        rm_count_cond = rm_count <= right_motor_degrees;

        if(counter_clockwise)
        {
            lm_count_cond = !lm_count_cond;
            rm_count_cond = !rm_count_cond;
        }

        if(lm_count_cond)
        {
            nxt_motor_set_speed(LEFT_MOTOR, MOTOR_NO_SPEED, true);
            motor_one_running = false;
        }

        if(rm_count_cond)
        {
            nxt_motor_set_speed(RIGHT_MOTOR, MOTOR_NO_SPEED, true);
            motor_two_running = false;
        }       
    }
}
