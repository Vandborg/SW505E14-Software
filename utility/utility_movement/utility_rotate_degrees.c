// Third party libraries
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"

// Own header
#include "utility_rotate_degrees.h"

// Own libraries
#include "utility/utility_definitions/utility_definitions.h"

#define TURN_TIMEOUT 100
#define DIRECTION 1
#define REVERSE_DIRECTION -1

// Function that rotates one engine a specific amount of degrees at given speed
void motor_rotate_degrees(int degrees, int speed, U32 motor)
{
    // Get the starting degrees for the motor, set current to the same value.
    int start_degrees = nxt_motor_get_count(motor);
    int current_degrees = start_degrees;

    // Initialize direction
    int direction = 0;

    // Check whether both degrees and speed are either positive or negative.
    // If they are, set direction to forward, else direction is backward.
    if((degrees < 0 && speed < 0) || (degrees > 0 && speed > 0))
    {
        direction = DIRECTION;
    }
    else
    {
        direction = REVERSE_DIRECTION;
    }

    // Checks if the speed is set to 0. If so, skip the entire thing.
    if(speed != MOTOR_NO_SPEED)
    {

        // Numeric value of speed
        if(speed < 0)
        {
            speed *= -1;
        }

        // Numeric value of degrees
        if(degrees < 0)
        {
            degrees *= -1;
        }

        // Compute the target degrees for the rotation.
        int target_degrees = start_degrees + degrees * direction;
        
        // Since we have |speed|, multiply by the desired direction.
        speed *= direction;

        // Checks if the target is after the current - forward direction
        if(target_degrees > current_degrees)
        {
            while(target_degrees > current_degrees)
            {
                nxt_motor_set_speed(motor, speed, BRAKE);
                current_degrees = nxt_motor_get_count(motor);
            }   
        }
        // Checks if the target is before the current - reverse direction
        else if(target_degrees < current_degrees)
        {
            while(target_degrees < current_degrees)
            {
                nxt_motor_set_speed(motor, speed, BRAKE);
                current_degrees = nxt_motor_get_count(motor);
            }
        }
    }

    // Brake when the rotation has been complete.
    nxt_motor_set_speed(motor, MOTOR_NO_SPEED, BRAKE);
    systick_wait_ms(TURN_TIMEOUT);
}

// Turns a specific motor a given amount of rotations at a specific speed.
// The mode can be used to indicate how many fractions of a whole rotation you
// want to turn. Defines are made for this in utility_definions.h. 
// Use this to avoid having to compute how many degrees correspond to a specific
// amount of rotations on your own. 
void motor_rotate_rotations(int rotations, int mode, int speed, U32 motor)
{
    motor_rotate_degrees((rotations*360)/mode, speed, motor);
}
