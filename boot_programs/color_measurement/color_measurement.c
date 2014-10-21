// Third party libraries
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"
#include "string.h"

// Own header
#include "boot_programs/color_measurement/color_measurement.h"

// Own libraries
#include "utility/utility_definitions/utility_definitions.h"
#include "utility/utility_lcd/utility_lcd.h"
#include "utility/utility_sound/utility_sound.h"
#include "utility/utility_string/utility_string.h"
#include "utility/utility_structs/utility_structs.h"

void enter_measurement_mode(void)
{
    lcd_display_line(LCD_LINE_EIGHT, "      EXIT  HOLD", true);
}

void enter_normal_mode(void)
{
    lcd_display_line(LCD_LINE_EIGHT, "      EXIT  SCAN", true);
}

// Will reset the menu
void reset_menu(void)
{
    // Create menu
    char menu[LCD_HEIGHT][LCD_WIDTH + 1] = {"  MEASURE COLOR ",
                                            "================",
                                            "                ",
                                            "                ",
                                            "                ",
                                            "                ",
                                            "================",
                                            "                "};

    lcd_display_lines(LCD_LINE_ONE, LCD_HEIGHT, menu, true); // Display the menu
}

void display_color_measurements(color left, color right)
{
    char s[17];

    // Add indentificators for the color measurements
    lcd_display_line(LCD_LINE_FOUR, "R:        R:    ", false);
    lcd_display_line(LCD_LINE_FIVE, "G:        G:    ", false);
    lcd_display_line(LCD_LINE_SIX,  "B:        B:    ", false);

    // Add the concrete measurements
    lcd_display_string_at_column(LCD_LINE_FOUR, LCD_COLUMN_FOUR, 
                                 int_to_string(left.red, s), false, false);
    lcd_display_string_at_column(LCD_LINE_FIVE, LCD_COLUMN_FOUR, 
                                 int_to_string(left.green, s), false, false);
    lcd_display_string_at_column(LCD_LINE_SIX, LCD_COLUMN_FOUR, 
                                 int_to_string(left.blue, s), false, false);

    lcd_display_string_at_column(LCD_LINE_FOUR, LCD_COLUMN_FOURTEEN, 
                                 int_to_string(right.red, s), false, false);
    lcd_display_string_at_column(LCD_LINE_FIVE, LCD_COLUMN_FOURTEEN, 
                                 int_to_string(right.green, s), false, false);
    lcd_display_string_at_column(LCD_LINE_SIX, LCD_COLUMN_FOURTEEN, 
                                 int_to_string(right.blue, s), false, true);
}

// Scan a color
color measure_color(U8 sensor)
{
    // Buffer for color measurement
    S16 buffer[3] = { 0, 0, 0 };

    // Allocate space for color measurement
    color result = { 0, 0, 0 };

    // Get the color from the sensor
    ecrobot_get_nxtcolorsensor_rgb(sensor, buffer);

    // Increment the color values
    result.red += buffer[0];
    result.green += buffer[1];
    result.blue += buffer[2];

    // Return the result
    return result;
}

void color_measurement(void)
{
    // Set the mode for the color sensors
    ecrobot_set_nxtcolorsensor(COLOR_SENSOR_LEFT, COLORSENSOR);
    ecrobot_set_nxtcolorsensor(COLOR_SENSOR_RIGHT, COLORSENSOR);

    color color_left = { 0, 0, 0 };
    color color_right = { 0, 0, 0 };
    color buffer = { 0, 0, 0 };

    bool do_measure_color = false;

    bool run_button_released = false;

    int measurements = 0;

    // Display menu
    reset_menu();
    enter_normal_mode();

    while(true)
    {
        // Check if the user pressed the run button
        if(ecrobot_is_RUN_button_pressed() && run_button_released)
        {
            // Wait until the user releases the button again
            while(ecrobot_is_RUN_button_pressed()) 
            {
                // Wait 
            }

            run_button_released = false;

            // Test if we are already measuring
            if(do_measure_color) {
                // Reset the color
                color_left.red = 0;
                color_left.green = 0;
                color_left.blue = 0;

                color_right.red = 0;
                color_right.green = 0;
                color_right.blue = 0; 
            }
            else {
                // Begin color measurement
                do_measure_color = true;
            }

            enter_measurement_mode();
        }
        else if(ecrobot_is_RUN_button_pressed() == false) 
        {
            run_button_released = true;
        }

        // Check if the device should make mesurements
        if(do_measure_color)
        {
            // Measure color values from left sensor
            buffer = measure_color(COLOR_SENSOR_LEFT);

            // Update values
            color_left.red += buffer.red;
            color_left.green += buffer.green;
            color_left.blue += buffer.blue;

            // Measure color values from right sensor
            buffer = measure_color(COLOR_SENSOR_RIGHT);

            // Update values
            color_right.red += buffer.red;
            color_right.green += buffer.green;
            color_right.blue += buffer.blue;

            // If not first scan, calculate average of new and previous
            if(measurements > 0) {
                color_left.red /= 2;
                color_left.green /= 2;
                color_left.blue /= 2;

                color_right.red /= 2;
                color_right.green /= 2;
                color_right.blue /= 2;
            }

            // Wait when over some measurements
            if(measurements++ % 250 == 0)
            {
                // Update the display
                display_color_measurements(color_left, color_right);

                systick_wait_ms(500);
            }
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
