// Third party libraries
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"

// Own header
#include "utility_rotate_degrees.h"

// Own libraries
#include "utility/utility_definitions/utility_definitions.h"
#include "utility/utility_sound/utility_sound.h"
#include "utility/utility_lcd/utility_lcd.h"
#include "utility/utility_string/utility_string.h"

void motor_rotate_degrees(int degrees, int speed, char* motor)
{
    char* placeholder = null;
    int current_rotations = nxt_motor_get_count(motor);
    

    
    // Checks if the desired direction is forward.
    if(speed > MOTOR_NO_SPEED)
    {
        int target_rotations = current_rotations + degrees;
        // While the condition still holds, keep moving the fork up
        while(target_rotations > current_rotations)
        {
            nxt_motor_set_speed(motor, speed, BRAKE);
            current_rotations = nxt_motor_get_count(motor);
            placeholder = int_to_string(current_rotations, placeholder);
            lcd_display_line(LCD_LINE_ONE, placeholder, TRUE);
            systick_wait_ms(50);
        }

    }
    // Checks if the desired direction is backward. 
    else if(speed < MOTOR_NO_SPEED)
    {
        int target_rotations = current_rotations - degrees;
        // While the condition still holds, keep moving the fork down
        while(target_rotations < current_rotations)
        {
            nxt_motor_set_speed(motor, speed, BRAKE);
            current_rotations = nxt_motor_get_count(motor);
            placeholder = int_to_string(current_rotations, placeholder);
            lcd_display_line(LCD_LINE_ONE, placeholder, TRUE);
            systick_wait_ms(50);
        }        
    }

    // Once the desired height has been reached, stop the motor.
    nxt_motor_set_speed(motor, MOTOR_NO_SPEED, BRAKE);
}

// Turns a specific motor a given amount of rotations at a specific speed.
// The mode can be used to indicate how many fractions of a whole rotation you
// want to turn. Defines are made for this in utility_definions.h. 
// Use this to avoid having to compute how many degrees a specific amount of
// rotations are on your own. 
void motor_rotate_rotations(int rotations, int mode, int speed, char* motor)
{
    motor_rotate_degrees((rotations*360)/mode, speed, motor);
}
