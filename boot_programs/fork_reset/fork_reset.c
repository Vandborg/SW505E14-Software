// Third party libraries
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"

// Own header
#include "fork_reset.h"

// Own libraries
#include "utility/utility_definitions/utility_definitions.h"
#include "utility/utility_lcd/utility_lcd.h"
#include "utility/utility_sound/utility_sound.h"

#define LIFTING_SPEED 70
#define OPTIONAL_TOUCH_SENSOR NXT_PORT_S4

void fork_reset(void)
{

    while(ecrobot_is_RUN_button_pressed()) 
    {
        // Busy wait for run button to be released
    }

    char lifting_display[8][17] = {"----- LIFT -----", 
                                   "================",
                                   "                ",
                                   "[INSERT BUTTON ]",
                                   "[   IN PORT 4  ]",
                                   "[PRESS TO LIFT ]",
                                   "================",
                                   "      EXIT  MODE"};
    lcd_display_lines(LCD_LINE_ONE, LCD_HEIGHT, lifting_display, TRUE);

    int lifting_mode = TRUE; // lifting mode. False = lower, True = lift.

    while(TRUE)
    {
        // If the run button is pressed, change the lifting mode.
        if(ecrobot_is_RUN_button_pressed())
        {
            while(ecrobot_is_RUN_button_pressed())
            {
                // Busy wait for run button to be released
            }
            play_sound(SOUND_BUTTON_FEEDBACK);
            lifting_mode = !lifting_mode;
            change_lifting_display_mode(lifting_mode);
        }
        // If the touch sensor is pressed, either lift or lower the fork.
        else if(ecrobot_get_touch_sensor(OPTIONAL_TOUCH_SENSOR) &&
                !ecrobot_is_RUN_button_pressed())
        {
            // Lifting mode is true. This lifts the fork.
            if(lifting_mode)
            {
                while(ecrobot_get_touch_sensor(OPTIONAL_TOUCH_SENSOR))
                {
                    nxt_motor_set_speed(FORK_MOTOR, LIFTING_SPEED, NO_BRAKE);
                    systick_wait_ms(50);
                }
                nxt_motor_set_speed(FORK_MOTOR, MOTOR_NO_SPEED, BRAKE);
            }
            // Lifting mode is false. This lowers the fork.
            else
            {
                while(ecrobot_get_touch_sensor(OPTIONAL_TOUCH_SENSOR))
                {
                    nxt_motor_set_speed(FORK_MOTOR, -LIFTING_SPEED, NO_BRAKE);
                    systick_wait_ms(50);
                }
                nxt_motor_set_speed(FORK_MOTOR, MOTOR_NO_SPEED, BRAKE);
            }
        }
        // If the enter button is held down. Exit to call site. 
        else if(ecrobot_is_ENTER_button_pressed())
        {   
            // Time when the button was pressed
            int enter_button_pressed_start_time = systick_get_ms();

            // Continusly check if enough time has passed to exit
            while(ecrobot_is_ENTER_button_pressed())
            {
                if(enter_button_pressed_start_time + ENTER_BUTTON_EXIT_TIMEOUT <
                   systick_get_ms()) 
                {   
                    // Set fork motor count to 0,
                    // so the fork is completely reset.
                    nxt_motor_set_count(FORK_MOTOR, 0);
                    return;
                }
            }
        }
    }
}

// Function used to display whether the fork currently lifts or lowers.
void change_lifting_display_mode(int lifting_mode)
{
    // If the lifting mode is true, the display will show that you are lifting
    if(lifting_mode)
    {
        lcd_display_line(LCD_LINE_ONE,   "----- LIFT -----", FALSE);
        lcd_display_line(LCD_LINE_SIX,   "[PRESS TO LIFT ]", TRUE);
    }
    // If the lifting mode is false, the display will show that you are lowering
    else
    {
        lcd_display_line(LCD_LINE_ONE,   "----- LOWER ----", FALSE);
        lcd_display_line(LCD_LINE_SIX,   "[PRESS TO LOWER]", TRUE);
    }
}
