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
#define FADING_SPEED_REDUCTION 1.10// This is an arbitrary value. Change if need
#define SLOW_SPEED_REDUCTION 1.03  // This is an arbitrary value. Change if need
#define MOTOR_LOWER_SPEED_LIMIT 10 // This is an arbitrary value. Change if need
#define MOTOR_NO_SPEED 0
#define BRAKE 1
#define NO_BRAKE 0
#define SMALL_WAIT_MS 100          // This is an arbitrary value. Change if need

// Main function that that brakes the forklift. Call this to brake.
void forklift_brake(int brakepower, int left_motor_speed, int right_motor_speed)
{
    switch(brakepower)
    {
        case HIGH_BRAKEPOWER:
            fullstop_brake();
            break;
        case MEDIUM_BRAKEPOWER:
            fading_brake(brakepower, left_motor_speed, right_motor_speed);
            break;
        case LOW_BRAKEPOWER:
            slow_brake(brakepower, left_motor_speed, right_motor_speed);
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
    
    // small wait to make sure it has time to reduce the speed to 0.
    systick_wait_ms(SMALL_WAIT_MS);
}

// Function that reduces both motors' speed gradually from current speed to 0. 
void fading_brake(int brakepower, int left_motor_speed, int right_motor_speed)
{
    /* Loops while either left or right motor is above the speed limit in either
     * reverse or normal driving direction
     */
    while(left_motor_speed > MOTOR_LOWER_SPEED_LIMIT ||
          left_motor_speed < -MOTOR_LOWER_SPEED_LIMIT || 
          right_motor_speed > MOTOR_LOWER_SPEED_LIMIT ||
          right_motor_speed < -MOTOR_LOWER_SPEED_LIMIT)
    {
        // Compute the reduced speed which is the old speed divided by a factor
        left_motor_speed = left_motor_speed / FADING_SPEED_REDUCTION;
        right_motor_speed = right_motor_speed / FADING_SPEED_REDUCTION;

        // Set the motors to run at the reduced speeds.
        nxt_motor_set_speed(LEFT_MOTOR, left_motor_speed, BRAKE);
        nxt_motor_set_speed(RIGHT_MOTOR, right_motor_speed, BRAKE);

        // small wait to make sure it drives a bit at the reduced speed before
        // reducing the speed further.
        systick_wait_ms(SMALL_WAIT_MS);
    }

    // Makes a full stop brake at the end, to make sure it doesn't keep rolling.
    fullstop_brake();

} 

// Function that reduces both motors' speed slowly from current speed to 0.
void slow_brake(int brakepower, int left_motor_speed, int right_motor_speed)
{
    /* Loops while either left or right motor is above the speed limit in either
     * reverse or normal driving direction
     */
     while(left_motor_speed > MOTOR_LOWER_SPEED_LIMIT ||
           left_motor_speed < -MOTOR_LOWER_SPEED_LIMIT || 
           right_motor_speed > MOTOR_LOWER_SPEED_LIMIT ||
           right_motor_speed < -MOTOR_LOWER_SPEED_LIMIT)
    {
        // Compute the reduced speed which is the old speed divided by a factor
        left_motor_speed = left_motor_speed / SLOW_SPEED_REDUCTION;
        right_motor_speed = right_motor_speed / SLOW_SPEED_REDUCTION;

        // Set the motors to run at the reduced speeds.
        nxt_motor_set_speed(LEFT_MOTOR, left_motor_speed, BRAKE);
        nxt_motor_set_speed(RIGHT_MOTOR, right_motor_speed, BRAKE);

        // small wait to make sure it drives a bit at the reduced speed before
        // reducing the speed further.
        systick_wait_ms(SMALL_WAIT_MS);
        
    }

    // Makes a full stop brake at the end, to make sure it doesn't keep rolling.
    fullstop_brake();

}
