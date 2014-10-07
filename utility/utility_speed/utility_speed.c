// Third party libraries
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"

// Own header
#include "utility/utility_speed/utility_speed.h"

// Own libraries
#include "utility/utility_definitions/utility_definitions.h"

// Directions
#define DIRECTION_POSITIVE 1
#define DIRECTION_NEGATIVE -1

// Slope definitions
#define SLOPE_INTERVAL 100 // Amount of steps before changing the speed
#define SLOPE_RANGE SLOPE_INTERVAL * 5

// Speed definitions
#define SPEED_NOT_SET -1

// Checks if the current is within a range
bool is_within_range(int start, int current, int end)
{
    // START ---- CURRENT ---- TARGET
    if(start <= current && current <= end)
    {
        return true;
    }

    // TARGET ---- CURRENT ---- START
    else if(start >= current && current >= end)
    {
        return true;
    }

    return false;
}

int calculate_easing_speed(int start_mc, int end_mc, int current_mc)
{
    // Array of speed interval indexes 
    int speeds[] = {50, 60, 70, 80, 90};

    int distance = 0; // The distance from start to end
    int relative_current = 0; // Relative motor count from start to current
    int speed_start_slope = SPEED_NOT_SET; // Speed at start slope
    int speed_end_slope = SPEED_NOT_SET;// Speed at end slope
    int direction = 0; // Direction of motor
    int return_speed = 0; // Speed to be returned

    // Check and validate the direction the motor needs to go
    if(start_mc < end_mc)
    {  
        // Throw an error if out of bounds
        if(current_mc < start_mc)
        {
            return SPEED_CURRENT_BEFORE_START;
        }
        else if(current_mc > end_mc)
        {
            return SPEED_CURRENT_AFTER_END;
        }

        // Update values according to start_mc is lower than end_mc
        distance = end_mc - start_mc; 
        relative_current = current_mc - start_mc;
        direction = DIRECTION_POSITIVE;
    }
    else if(end_mc < start_mc)
    {
        // Throw error if out of bounds
        if(current_mc > start_mc)
        {
            return SPEED_CURRENT_BEFORE_START;
        }
        else if(current_mc < end_mc)
        {
            return SPEED_CURRENT_AFTER_END;
        }

        // Update values according to start_mc is greater than end_mc
        distance = start_mc - end_mc;
        relative_current = current_mc - end_mc;
        direction = DIRECTION_NEGATIVE; 
    }
    else
    {
        return MOTOR_NO_SPEED; // You are where you want to be
    }   

    // The point where the ending slope starts (ease out)
    int end_slope_start = distance - SLOPE_RANGE;

    // If the current motor count is not easing in or out
    if(is_within_range(SLOPE_RANGE, relative_current, end_slope_start))
    {
        // Return full motor speed corresponding to the right direction
        return MOTOR_FULL_SPEED * direction;
    }
    
    // If the current motor count is easing in
    if(relative_current < SLOPE_RANGE)
    {
        speed_start_slope = speeds[relative_current / SLOPE_INTERVAL];
    }
    
    // If the current motor count is easing out
    if(relative_current > end_slope_start)
    {
        // Look in the array in opposite direction
        speed_end_slope = speeds[(sizeof(speeds) / sizeof(int) - 1 )- 
                                 (relative_current - end_slope_start) / 
                                 SLOPE_INTERVAL];
    }

    // Return the smallest value of the easing in and out
    if(speed_start_slope != SPEED_NOT_SET && speed_end_slope != SPEED_NOT_SET)
    {
        // Return the smallest value
        return_speed = speed_start_slope < end_slope_start ? 
                       speed_start_slope : end_slope_start;
    }
    else if(speed_start_slope != SPEED_NOT_SET)
    {
        return_speed = speed_start_slope;
    }
    else if(speed_end_slope != SPEED_NOT_SET)
    {
        return_speed = speed_end_slope;
    }

    return return_speed * direction; // Return calculated easing speed
}
