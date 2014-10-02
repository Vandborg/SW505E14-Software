#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"
#include "string.h"

#include "utility_braking.h"
#include "../utility_definitions/utility_definitions.h"

#define HIGH_BRAKEPOWER 2
#define MEDIUM_BRAKEPOWER 1
#define LOW_BRAKEPOWER 0
#define FADING_SPEED_REDUCTION 1.10
#define SLOW_SPEED_REDUCTION 1.03
#define MOTOR_LOWER_SPEED_LIMIT 10
#define MOTOR_NO_SPEED 0
#define BRAKE 1
#define NO_BRAKE 0

/* 
 * Function that brakes the forklift.
 * brakepower       The desired brake power. Ranges from 0 to 2, low to hard.
 * forklift_speed   The current speed of the forklift. Ranges from -100 to 100.
*/
void forklift_brake(int brakepower, int forklift_speed)
{
    switch(brakepower)
    {
        case HIGH_BRAKEPOWER:
            fullstop_brake();
            break;
        case MEDIUM_BRAKEPOWER:
            fading_brake(brakepower, forklift_speed);
            break;
        case LOW_BRAKEPOWER:
            slow_brake(brakepower, forklift_speed);
            break;
        default :
            break;
    }
}

/* Sets both wheel motors to zero speed and applies brake.*/
void fullstop_brake(void)
{
    nxt_motor_set_speed(LEFT_MOTOR, MOTOR_NO_SPEED, BRAKE);
    nxt_motor_set_speed(RIGHT_MOTOR, MOTOR_NO_SPEED, BRAKE);
    systick_wait_ms(100);
}

void fading_brake(int brakepower, int forklift_speed)
{
    if(forklift_speed > MOTOR_NO_SPEED)
    {
        while(forklift_speed > MOTOR_LOWER_SPEED_LIMIT)
        {
            nxt_motor_set_speed(LEFT_MOTOR, 
                                forklift_speed / FADING_SPEED_REDUCTION,
                                BRAKE);
            nxt_motor_set_speed(RIGHT_MOTOR, 
                                forklift_speed / FADING_SPEED_REDUCTION,
                                BRAKE);
            systick_wait_ms(100);
            forklift_speed = forklift_speed / FADING_SPEED_REDUCTION;
        }
        fullstop_brake();

    }
    else if(forklift_speed < MOTOR_NO_SPEED)
    {
        while(forklift_speed < -MOTOR_LOWER_SPEED_LIMIT)
        {
            nxt_motor_set_speed(LEFT_MOTOR, 
                                forklift_speed / FADING_SPEED_REDUCTION,
                                BRAKE);
            nxt_motor_set_speed(RIGHT_MOTOR, 
                                forklift_speed / FADING_SPEED_REDUCTION,
                                BRAKE);
            systick_wait_ms(100);
            forklift_speed = forklift_speed / FADING_SPEED_REDUCTION;
        }
        fullstop_brake();
    }
    else
    {
        //Wheel-motors not moving
    }
}

void slow_brake(int brakepower, int forklift_speed)
{
    if(forklift_speed > MOTOR_NO_SPEED)
    {   
        while(forklift_speed > MOTOR_LOWER_SPEED_LIMIT)
        {
            nxt_motor_set_speed(LEFT_MOTOR, 
                                forklift_speed / SLOW_SPEED_REDUCTION,
                                BRAKE);
            nxt_motor_set_speed(RIGHT_MOTOR, 
                                forklift_speed / SLOW_SPEED_REDUCTION,
                                BRAKE);
            systick_wait_ms(100);
            forklift_speed = forklift_speed / SLOW_SPEED_REDUCTION;
        }

        fullstop_brake();

    }
    else if(forklift_speed < MOTOR_NO_SPEED)
    {
        while(forklift_speed < -MOTOR_LOWER_SPEED_LIMIT)
        {
            nxt_motor_set_speed(LEFT_MOTOR, 
                                forklift_speed / SLOW_SPEED_REDUCTION,
                                BRAKE);
            nxt_motor_set_speed(RIGHT_MOTOR, 
                                forklift_speed / SLOW_SPEED_REDUCTION,
                                BRAKE);
            systick_wait_ms(100);
            forklift_speed = forklift_speed / SLOW_SPEED_REDUCTION;
        }
        fullstop_brake();
    }
    else
    {
        //Wheel-motors not moving
    }
}
