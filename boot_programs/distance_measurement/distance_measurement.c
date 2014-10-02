// Third party libraries
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"

// Own header
#include "distance_measurement.h"

// Own libraries
#include "utility/utility_definitions/utility_definitions.h"
#include "utility/utility_lcd/utility_lcd.h"
#include "utility/utility_sound/utility_sound.h"

#define SONAR_SCAN_DELAY 200 // Ms inbetween scans from sensor

void distance_measurement(void)
{
    // Prepare the display
    display_clear(TRUE);
    lcd_display_line(LCD_LINE_ONE,   "  MEASURE COLOR ", FALSE);
    lcd_display_line(LCD_LINE_TWO,   "================", FALSE);
    lcd_display_line(LCD_LINE_FOUR,  "CURRENT:        ", FALSE);
    lcd_display_line(LCD_LINE_FIVE,  "LOCK:           ", FALSE);
    lcd_display_line(LCD_LINE_SEVEN, "================", FALSE);
    lcd_display_line(LCD_LINE_EIGHT, "      EXIT  LOCK", TRUE);

    int run_button_released = TRUE; // Boolean to check if run button is pressed
    int sensor_output = 0; // The output from the sonar sensor

    // Loop until the user breaks the loop by holding the enter button
    while(TRUE)
    {
        // Update the sonar output when its time to do so
        if(systick_get_ms() % SONAR_SCAN_DELAY == 0)
        {
            sensor_output = ecrobot_get_sonar_sensor(SONAR_SENSOR);
            display_goto_xy(9, 3);
            display_int(sensor_output, 3);
            display_update();
        }

        // Wait for the user to press the run button
        if(ecrobot_is_RUN_button_pressed() && run_button_released)
        {
            run_button_released = FALSE; // Set the run button to be pressed
            play_sound(SOUND_BUTTON_FEEDBACK); // Play button feedback

            // Update the sensor values on the display
            display_goto_xy(9, 4);
            display_int(sensor_output, 3);
            display_update();  
        }
        // Has the enter button been released
        else if(ecrobot_is_RUN_button_pressed() == FALSE)
        {
            run_button_released = TRUE;
        }
        
        // Chek if the enter button is pressed for long enough time to exit
        if(ecrobot_is_ENTER_button_pressed())
        {   
            // Time when the button was pressed
            int enter_button_pressed_start_time = systick_get_ms();

            // Continusly check if enought time has past to exit
            while(ecrobot_is_ENTER_button_pressed())
            {
                if(enter_button_pressed_start_time + ENTER_BUTTON_EXIT_TIMEOUT <
                   systick_get_ms()) 
                {   
                    return;
                }
            }
        }
    }
}
