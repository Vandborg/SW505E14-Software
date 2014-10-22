// Third party libraries
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"
#include "string.h"

// Own header
#include "boot_programs/color_calibration/color_calibration.h"

// Own libraries
#include "utility/utility_definitions/utility_definitions.h"
#include "utility/utility_lcd/utility_lcd.h"
#include "utility/utility_movement/utility_braking.h"
#include "utility/utility_movement/utility_distance.h"
#include "utility/utility_movement/utility_drive_straight.h"
#include "utility/utility_sound/utility_sound.h"
#include "utility/utility_string/utility_string.h"
#include "utility/utility_structs/utility_structs.h"

// Number of scans to be executed
#define NUMBER_OF_COLOR_SCANS 1000

// Distance to move between colors
#define DISTANCE_BETWEEN_MEASUREMENTS 6

// Global variables to use
color Color_left[AMOUNT_OF_COLORS];
color Color_right[AMOUNT_OF_COLORS];

void enter_scanning_mode(void)
{
    lcd_display_line(LCD_LINE_EIGHT, "--- Scanning ---", true);
}

void enter_normal_mode(void)
{
    lcd_display_line(LCD_LINE_EIGHT, "      EXIT  SCAN", true);
}

// Will reset the menu
void reset_menu(void)
{
    // Create menu
    char menu[LCD_HEIGHT][LCD_WIDTH + 1] = {" CALIBRATE COLOR",
                                            "================",
                                            "                ",
                                            "                ",
                                            "                ",
                                            "                ",
                                            "================",
                                            "                "};

    lcd_display_lines(LCD_LINE_ONE, LCD_HEIGHT, menu, true); // Display the menu
}

void display_colors(color left, color right, char* color_name)
{
    // Buffer to string manipulation
    char s[80];

    // Display the name of the color
    lcd_display_line(LCD_LINE_THREE, color_name, false);

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
color scan_color(U8 sensor)
{
    // Buffer for color measurement
    S16 buffer[3] = { 0, 0, 0 };

    // Allocate space for color measurement
    color result = { 0, 0, 0 };

    // Measure the color and add to result
    for(int i = 0; i < NUMBER_OF_COLOR_SCANS; i++)
    {
        // Get the color from the sensor
        ecrobot_get_nxtcolorsensor_rgb(sensor, buffer);

        // Increment the color values
        result.red += buffer[0];
        result.green += buffer[1];
        result.blue += buffer[2];
    }

    // Find avarage of all measurements
    result.red /= NUMBER_OF_COLOR_SCANS;
    result.green /= NUMBER_OF_COLOR_SCANS;
    result.blue /= NUMBER_OF_COLOR_SCANS;

    // Return the result
    return result;
}

char* get_color_name(int color_id, char* buffer)
{
    switch(color_id)
    {
        case COLOR_RED: 
            buffer = "RED"; 
            break;
        case COLOR_BLUE: 
            buffer = "BLUE"; 
            break;
        case COLOR_BLACK: 
            buffer = "BLACK"; 
            break;
        case COLOR_GRAY: 
            buffer = "GRAY"; 
            break;
        case COLOR_WHITE: 
            buffer = "WHITE"; 
            break;
    }

    return buffer;
}

void color_calibration(void)
{
    // Buffer variables for color scanning
    color color_left = { -1, -1, -1 };
    color color_right = { -1, -1, -1 };

    // Boolean to check if run button is released
    int run_button_released = true;

    // Set the mode for the color sensors
    ecrobot_set_nxtcolorsensor(COLOR_SENSOR_LEFT, NXT_COLORSENSOR);
    ecrobot_set_nxtcolorsensor(COLOR_SENSOR_RIGHT, NXT_COLORSENSOR);

    // Prepare the display
    display_clear(true);

    // Reset the menu
    reset_menu();

    // Enter normal mode (exit / scan menu)
    enter_normal_mode();

    // Loop until the user breaks the loop by holding the enter button
    while(true)
    {
        // Wait for the user to press the run button
        if(ecrobot_is_RUN_button_pressed() && run_button_released)
        {
            run_button_released = false; // The button is pressed
            play_sound(SOUND_BUTTON_FEEDBACK); // Play button feedback

            char buf[17];

            // Go through all colors
            for(int i = 0; i < AMOUNT_OF_COLORS; i++)
            {
                // Reset the menu 
                reset_menu();

                // Play wait sound
                play_sound(SOUND_MODE_WAIT);

                // Replace bottom of the menu
                enter_scanning_mode();

                // Scan the colors
                color_left = scan_color(COLOR_SENSOR_LEFT);
                color_right = scan_color(COLOR_SENSOR_RIGHT);

                // Update global variables
                Color_left[i] = color_left;
                Color_right[i] = color_right;

                // Display the colors to the display
                display_colors(color_left, color_right, get_color_name(i, buf));

                // Go back to normal mode
                enter_normal_mode();

                // Notify that the scan is done
                play_sound(SOUND_NOTIFICATION);

                // Wait some time before next scan
                systick_wait_ms(500);

                // Do not move on the last iteration
                if(i != AMOUNT_OF_COLORS - 1) 
                {
                    // Reset distance
                    reset_distance();

                    // Move forward
                    drive_forward();

                    // Wait until disired length has been driven
                    while(current_distance() < DISTANCE_BETWEEN_MEASUREMENTS)
                    {
                        // Do nothing (wait)
                    }

                    // Brake (Update motor speeds when we can get them)
                    forklift_brake(HIGH_BRAKEPOWER, 100, 100);

                    // Wait before scanning
                    systick_wait_ms(500);
                }
            }

            // Reset the menu
            reset_menu();
            enter_normal_mode();

        }
        // Has the enter button been released
        else if(ecrobot_is_RUN_button_pressed() == false)
        {
            run_button_released = true;
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
