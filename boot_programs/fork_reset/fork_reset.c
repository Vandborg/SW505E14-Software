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
#include "utility/utility_movement/utility_distance.h"


#define LIFTING_SPEED 70
#define OPTIONAL_TOUCH_SENSOR NXT_PORT_S4

void fork_reset(void)
{

    while(ecrobot_is_RUN_button_pressed()) 
    {
        // Busy wait for run button to be released
    }

    char lifting_display[8][17] = {"- LIFT / LOWER -", 
                                   "================",
                                   "[ ROTATE RIGHT ]",
                                   "[   WHEEL TO   ]",
                                   "[ LIFT / LOWER ]",
                                   "[   THE FORK   ]",
                                   "================",
                                   "      EXIT      "};
    lcd_display_lines(LCD_LINE_ONE, LCD_HEIGHT, lifting_display, true);

    nxt_motor_set_count(RIGHT_MOTOR, 0);
    nxt_motor_set_count(FORK_MOTOR, 0);

    while(true)
    {
        if(ecrobot_is_ENTER_button_pressed())
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

        int right_count = nxt_motor_get_count(RIGHT_MOTOR);
        int fork_count = nxt_motor_get_count(FORK_MOTOR);

        if (right_count > fork_count + 2)
        {
            nxt_motor_set_speed(FORK_MOTOR, LIFTING_SPEED, 0);
        }
        else if(right_count < fork_count - 2)
        {
            nxt_motor_set_speed(FORK_MOTOR, -LIFTING_SPEED, 0);
        }
        else
        {
            nxt_motor_set_speed(FORK_MOTOR, 0, 1);
        }
    }
}
