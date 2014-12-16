// Third party libraries
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"

// Own libraries
#include "utility/utility_definitions/utility_definitions.h"

#define WHEEL_CIRCUMFERENCE_MM 134
#define MM_TO_CM 10
#define DEGREES_IN_CIRCLE 360

void reset_distance(void); 
int current_distance(void);


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
                        DEGREES_IN_CIRCLE;

    int distance_right = (nxt_motor_get_count(RIGHT_MOTOR) - 
                         start_motor_count_right) *
                         WHEEL_CIRCUMFERENCE_MM / 
                         DEGREES_IN_CIRCLE;

    int distance_average = (distance_left + distance_right) / 2;

    return distance_average;
}


void drive_straight_distance(int distance) {
    
    int Kp = 10;
    int Ki = 1;
    int Kd = 12;

    int error = 0;
    int integral_straight = 0;
    int last_error = 0;
    int derivative = 0;

    int init_motor_count_left = nxt_motor_get_count(LEFT_MOTOR);
    int init_motor_count_right = nxt_motor_get_count(RIGHT_MOTOR);

    int current_count_left = 0;
    int current_count_right = 0;

    int output = 0;

    int powerA = 0;
    int powerB = 0;

    reset_distance();

    while(current_distance() < distance)
    {
        current_count_right = 
            nxt_motor_get_count(RIGHT_MOTOR) - init_motor_count_right;
        current_count_left = 
            nxt_motor_get_count(LEFT_MOTOR) - init_motor_count_left;

        error = current_count_right - current_count_left;
        integral_straight = (2/3) * integral_straight + error;
        derivative = error - last_error;

        output = Kp * error + Ki * integral_straight + Kd * derivative;

        powerA = LINE_FOLLOW_SPEED - output;
        powerB = LINE_FOLLOW_SPEED + output;

        nxt_motor_set_speed(RIGHT_MOTOR, powerA, 1);
        nxt_motor_set_speed(LEFT_MOTOR, powerB, 1);
    }

    // Brake (Update motor speeds when we can get them)
    nxt_motor_set_speed(RIGHT_MOTOR, 0, 1);
    nxt_motor_set_speed(LEFT_MOTOR, 0, 1);
}
