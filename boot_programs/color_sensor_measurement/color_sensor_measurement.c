// Third party libraries
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"
#include "string.h"

// Own header
#include "boot_programs/color_sensor_measurement/color_sensor_measurement.h"

// Own libraries
#include "utility/utility_definitions/utility_definitions.h"
#include "utility/utility_lcd/utility_lcd.h"
#include "utility/utility_sound/utility_sound.h"
#include "utility/utility_string/utility_string.h"
#include "utility/utility_structs/utility_structs.h"

#define AMOUNT_OF_MODES 2
#define MEASUREMENT_WAIT 500
#define MEASUREMENT_WAIT_INTERVAL 250

int sensor_mode = 0; // Standard is color sensor
U8 sensor_modes[AMOUNT_OF_MODES] = { NXT_COLORSENSOR, NXT_LIGHTSENSOR_RED };

void enter_mode(U8 sensor_mode)
{
    switch(sensor_mode)
    {
        case NXT_COLORSENSOR: 
            lcd_display_line(LCD_LINE_ONE, " MEASURE: COLOR ", true);
            break;
        case NXT_LIGHTSENSOR_RED:
            lcd_display_line(LCD_LINE_ONE, " MEASURE: LIGHT ", true);
            break;
        default:
            lcd_display_line(LCD_LINE_ONE, "    MEASURE     ", true);
            break;
    }
}

// Will reset the menu
void reset_measurement_menu(void)
{
    // Create menu
    char menu[LCD_HEIGHT][LCD_WIDTH + 1] = {"                ",
                                            "================",
                                            "                ",
                                            "                ",
                                            "                ",
                                            "                ",
                                            "================",
                                            "      EXIT  MODE"};

    lcd_display_lines(LCD_LINE_ONE, LCD_HEIGHT, menu, true); // Display the menu
}

void display_color_sensor_measurements(color left, color right)
{
    char s[17];

    // Add indentificators for the color measurements
    lcd_display_line(LCD_LINE_FOUR, "R:       R:    ", false);
    lcd_display_line(LCD_LINE_FIVE, "G:       G:    ", false);
    lcd_display_line(LCD_LINE_SIX,  "B:       B:    ", false);

    // Add the concrete measurements
    lcd_display_string_at_column(LCD_LINE_FOUR, LCD_COLUMN_FOUR, 
                                 int_to_string(left.red, s), false, false);
    lcd_display_string_at_column(LCD_LINE_FIVE, LCD_COLUMN_FOUR, 
                                 int_to_string(left.green, s), false, false);
    lcd_display_string_at_column(LCD_LINE_SIX, LCD_COLUMN_FOUR, 
                                 int_to_string(left.blue, s), false, false);

    lcd_display_string_at_column(LCD_LINE_FOUR, LCD_COLUMN_THIRTEEN, 
                                 int_to_string(right.red, s), false, false);
    lcd_display_string_at_column(LCD_LINE_FIVE, LCD_COLUMN_THIRTEEN, 
                                 int_to_string(right.green, s), false, false);
    lcd_display_string_at_column(LCD_LINE_SIX, LCD_COLUMN_THIRTEEN, 
                                 int_to_string(right.blue, s), false, true);
}

void display_light_measurements(U16 left, U16 right)
{
    char s[17];

    // Add indentificators for the color measurements
    lcd_display_line(LCD_LINE_FIVE, "L:       R:     ", false);

    // Add the concrete measurements
    lcd_display_string_at_column(LCD_LINE_FIVE, LCD_COLUMN_FOUR, 
                                 int_to_string(left, s), false, false);

    lcd_display_string_at_column(LCD_LINE_FIVE, LCD_COLUMN_THIRTEEN, 
                                 int_to_string(right, s), false, true);
}

// Scan a color
color measure_color(U8 sensor)
{
    // Buffer for color measurement
    S16 buffer[3] = { 0, 0, 0 };

    // Allocate space for color measurement
    color result = { 0, 0, 0 };

    // Get the color from the sensor
    ecrobot_process_bg_nxtcolorsensor();
    ecrobot_get_nxtcolorsensor_rgb(sensor, buffer);

    // Increment the color values
    result.red += buffer[0];
    result.green += buffer[1];
    result.blue += buffer[2];

    // Return the result
    return result;
}

// Scan light 
U16 measure_light(U8 sensor)
{
    // Get the color from the sensor
    ecrobot_process_bg_nxtcolorsensor();
    return ecrobot_get_nxtcolorsensor_light(sensor);
}

void change_mode(U8 new_mode)
{
    // Set the mode for the color sensors
    ecrobot_set_nxtcolorsensor(COLOR_SENSOR_LEFT, new_mode);
    ecrobot_set_nxtcolorsensor(COLOR_SENSOR_RIGHT, new_mode);
    ecrobot_process_bg_nxtcolorsensor();
}

void color_sensor_measurement(void)
{
    // Variables used for color measurement
    color color_left = { 0, 0, 0 };
    color color_right = { 0, 0, 0 };
    color buffer = { 0, 0, 0 };
    int measurements = 0;

    // Variables used for light measurement
    U16 light_left = 0;
    U16 light_right = 0;

    // Button handling
    bool run_button_released = false;

    // Display menu
    reset_measurement_menu();

    // Enter the right mode
    enter_mode(sensor_modes[sensor_mode]);

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

            // Reset the menu
            reset_measurement_menu();

            // Change mode
            sensor_mode = (sensor_mode + 1) % AMOUNT_OF_MODES;
            change_mode(sensor_modes[sensor_mode]);

            // Enter the right mode
            enter_mode(sensor_modes[sensor_mode]);

            // Reset measurements
            measurements = 0;
        }
        else if(ecrobot_is_RUN_button_pressed() == false) 
        {
            run_button_released = true;
        }

        // Check if the device should make color mesurements
        if(sensor_modes[sensor_mode] == NXT_COLORSENSOR)
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

            // Calculate average of new and previous
            color_left.red /= 2;
            color_left.green /= 2;
            color_left.blue /= 2;

            color_right.red /= 2;
            color_right.green /= 2;
            color_right.blue /= 2;

            // Wait when over some measurements
            if(measurements++ % MEASUREMENT_WAIT_INTERVAL == 0)
            {
                // Update the display
                display_color_sensor_measurements(color_left, color_right);

                systick_wait_ms(MEASUREMENT_WAIT);
            }
        }
        else if (sensor_modes[sensor_mode] == NXT_LIGHTSENSOR_RED)
        {
            // Get measurements
            light_left += measure_light(COLOR_SENSOR_LEFT);
            light_right += measure_light(COLOR_SENSOR_RIGHT);

            // Get averages
            light_left /= 2;
            light_right /= 2;

            // Wait when over some measurements
            if(measurements++ % MEASUREMENT_WAIT_INTERVAL == 0)
            {
                // Update the display
                display_light_measurements(light_left, light_right);

                systick_wait_ms(MEASUREMENT_WAIT);
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
