// 3rd party includes
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"
#include "string.h"

// Own header include
#include "utility_braking.h"

// Own library includes
#include "../utility_definitions/utility_definitions.h"

// Locally needed definitions
#define FADING_SPEED_REDUCTION 1.10
#define SLOW_SPEED_REDUCTION 1.03
#define MOTOR_LOWER_SPEED_LIMIT 10
#define MOTOR_NO_SPEED 0
#define BRAKE 1
#define NO_BRAKE 0
#define SMALL_WAIT_MS 100

// Main function that that brakes the forklift. Call this to brake.
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

// Function that sets both motors to 0 speed and applies brake.
void fullstop_brake(void)
{
    nxt_motor_set_speed(LEFT_MOTOR, MOTOR_NO_SPEED, BRAKE);
    nxt_motor_set_speed(RIGHT_MOTOR, MOTOR_NO_SPEED, BRAKE);
    systick_wait_ms(SMALL_WAIT_MS);
}

// Function that reduces both motors' speed gradually from current speed to 0. 
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
            systick_wait_ms(SMALL_WAIT_MS);
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
            systick_wait_ms(SMALL_WAIT_MS);
            forklift_speed = forklift_speed / FADING_SPEED_REDUCTION;
        }
        fullstop_brake();
    }
    else
    {
        //Wheel-motors not moving
    }
}

//  Function that reduces both motors' speed slowly from current speed to 0.
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
            systick_wait_ms(SMALL_WAIT_MS);
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
            systick_wait_ms(SMALL_WAIT_MS);
            forklift_speed = forklift_speed / SLOW_SPEED_REDUCTION;
        }
        fullstop_brake();
    }
    else
    {
        //Wheel-motors not moving
    }
}
