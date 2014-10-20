// Third party libraries
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"

// Own libraries
#include "utility/utility_sound/utility_sound.h"
#include "utility/utility_string/utility_string.h"
#include "utility/utility_definitions/utility_definitions.h"
#include "utility/utility_lcd/utility_lcd.h"
#include "utility_braking.h"
#include "utility_distance.h"

#define SPEED 100
#define DISPLACED_STARTUP 60

void drive_forward(void)
{
    nxt_motor_set_speed(RIGHT_MOTOR, DISPLACED_STARTUP, FALSE);
    nxt_motor_set_speed(LEFT_MOTOR, SPEED, TRUE);
    nxt_motor_set_speed(RIGHT_MOTOR, SPEED, TRUE);
}

void drive_straight_pid(void) {
    int Kp = 18000;
    int Ki = 1;
    int Kd = 40000;
    int Tp = 80;

    int integral = 0;
    int currentError = 0;
    int lastError = 0;
    int derivative = 0;
    int turn = 0;

    int powerA = 0;
    int powerB = 0;
    int right_wheel_count = 0;
    int left_wheel_count = 0;
    int intial_rotation_right = nxt_motor_get_count(RIGHT_MOTOR);
    int intial_rotation_left = nxt_motor_get_count(LEFT_MOTOR);

    int initial_time = systick_get_ms();
    int current_time = 0;

    reset_distance();

    int flag = 1;

    while(true) 
    {
        if (flag)
        {
            left_wheel_count = 
            nxt_motor_get_count(LEFT_MOTOR) - intial_rotation_left;
            right_wheel_count = 
                nxt_motor_get_count(RIGHT_MOTOR) - intial_rotation_right;
            
            currentError = right_wheel_count - left_wheel_count;
            
            integral = integral + currentError;

            derivative = currentError - lastError;
            turn = Kp * currentError + Ki * integral + Kd * derivative;
            turn = turn / 10000; // ---> This is only needed if the k's 
                                 //      are multiplied by a hundred
            powerA = Tp + turn;
            powerB = Tp - turn;

            // Make motors go... 
            nxt_motor_set_speed(LEFT_MOTOR, powerA, 1);
            nxt_motor_set_speed(RIGHT_MOTOR, powerB, 1);

            lastError = currentError;

            current_time = systick_get_ms() - initial_time;
        }
        else 
        {
            right_wheel_count = 
                nxt_motor_get_count(RIGHT_MOTOR) - intial_rotation_right;
            left_wheel_count = 
                nxt_motor_get_count(LEFT_MOTOR) - intial_rotation_left;
            
            currentError = right_wheel_count - left_wheel_count;
            
            integral = integral + currentError;

            derivative = currentError - lastError;
            turn = Kp * currentError + Ki * integral + Kd * derivative;
            turn = turn / 10000; // ---> This is only needed if the k's 
                                 //      are multiplied by a hundred
            powerA = Tp + turn;
            powerB = Tp - turn;

            // Make motors go... 
            nxt_motor_set_speed(RIGHT_MOTOR, powerB, 1);
            nxt_motor_set_speed(LEFT_MOTOR, powerA, 1);

            lastError = currentError;

            current_time = systick_get_ms() - initial_time;
        }
        
        flag = !flag;
    }
}
    