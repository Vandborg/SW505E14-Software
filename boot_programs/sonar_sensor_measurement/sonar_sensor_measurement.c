// Third party libraries
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"
#include "string.h"

// Own header
#include "boot_programs/sonar_sensor_measurement/sonar_sensor_measurement.h"

// Own libraries
#include "utility/utility_definitions/utility_definitions.h"
#include "utility/utility_lcd/utility_lcd.h"
#include "utility/utility_sound/utility_sound.h"
#include "utility/utility_string/utility_string.h"
#include "utility/utility_structs/utility_structs.h"

#define MEASUREMENT_WAIT 500
#define MEASUREMENT_WAIT_INTERVAL 250

// Will reset the menu
void reset_sonar_measurement_menu(void)
{
    // Create menu
    char menu[LCD_HEIGHT][LCD_WIDTH + 1] = {" MEASURE: SONAR ",
                                            "================",
                                            "                ",
                                            "                ",
                                            "                ",
                                            "                ",
                                            "================",
                                            "      EXIT      "};

    lcd_display_lines(LCD_LINE_ONE, LCD_HEIGHT, menu, true); // Display the menu
}

void display_sonar_sensor_measurements(S32 front, S32 rear)
{
    char s[17];

    // Add indentificators for the distance measurements
    lcd_display_line(LCD_LINE_FIVE, "F:       R:    ", false);

    // Add the concrete measurements
    lcd_display_string_at_column(LCD_LINE_FIVE, LCD_COLUMN_FOUR, 
                                 int_to_string(front, s), false, false);

    lcd_display_string_at_column(LCD_LINE_FIVE, LCD_COLUMN_THIRTEEN, 
                                 int_to_string(rear, s), false, true);
}

void sonar_sensor_measurement(void)
{
    // Variables used for distance measurement
    S32 distance_left = 0;
    S32 distance_right = 0;
    int measurements = 0;

    // Display menu
    reset_sonar_measurement_menu();

    while(true)
    {
        // Update values
        distance_left += ecrobot_get_sonar_sensor(SONAR_SENSOR_FRONT);
        distance_right += ecrobot_get_sonar_sensor(SONAR_SENSOR_REAR);

        // Calculate average of new and previous
        distance_left /= 2;
        distance_right /= 2;

        // Wait when over some measurements
        if(measurements++ % MEASUREMENT_WAIT_INTERVAL == 0)
        {
            // Update the display
            display_sonar_sensor_measurements(distance_left, distance_right);

            systick_wait_ms(MEASUREMENT_WAIT);
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
