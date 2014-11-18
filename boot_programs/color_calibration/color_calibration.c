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
#include "utility/utility_bluetooth/utility_bluetooth.h"


// Number of scans to be executed
#define NUMBER_OF_COLOR_SCANS 1000

// Distance to move between colors
#define DISTANCE_BETWEEN_MEASUREMENTS 45

// Global variables to use
color Colors[AMOUNT_OF_COLORS];

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
        ecrobot_process_bg_nxtcolorsensor();
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
        case COLOR_RED_LEFT:
        case COLOR_RED_RIGHT: 
            buffer = "RED"; 
            break;
        case COLOR_BLUE_LEFT: 
        case COLOR_BLUE_RIGHT: 
            buffer = "BLUE"; 
            break;
        case COLOR_BLACK_LEFT:
        case COLOR_BLACK_RIGHT: 
            buffer = "BLACK"; 
            break;
        case COLOR_GRAY_LEFT:
        case COLOR_GRAY_RIGHT: 
            buffer = "GRAY"; 
            break;
        case COLOR_WHITE_LEFT:
        case COLOR_WHITE_RIGHT:
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
    ecrobot_process_bg_nxtcolorsensor();

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
            for(int i = 0; i < AMOUNT_OF_COLORS; i += 4)
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
                Colors[i] = color_left;
                Colors[i+1] = color_right;

                if(i == COLOR_BLACK_LEFT)
                {
                    drive_straight_distance(DISTANCE_BETWEEN_MEASUREMENTS);

                    // Wait before scanning
                    systick_wait_ms(500);

                    // Scan the colors
                    color_left = scan_color(COLOR_SENSOR_LEFT);
                    color_right = scan_color(COLOR_SENSOR_RIGHT);

                    // Get average of black tape and paper
                    Colors[i].red = 
                        (Colors[i].red + color_left.red) / 2;
                    Colors[i].green = 
                        (Colors[i].green + color_left.green) / 2;
                    Colors[i].blue = 
                        (Colors[i].blue + color_left.blue) / 2;

                    Colors[i+1].red = 
                        (Colors[i+1].red + color_right.red) / 2;
                    Colors[i+1].green = 
                        (Colors[i+1].green + color_right.green) / 2;
                    Colors[i+1].blue = 
                        (Colors[i+1].blue + color_right.blue) / 2;
                }

                // Display the colors to the display
                display_colors(Colors[i], Colors[i+1], get_color_name(i, buf));

                save_color_bt(i, 
                              Colors[i].red, 
                              Colors[i].green, 
                              Colors[i].blue);

                save_color_bt(i+1,
                              Colors[i+1].red, 
                              Colors[i+1].green, 
                              Colors[i+1].blue);

                // Notify that the scan is done
                play_sound(SOUND_NOTIFICATION);

                // Wait some time before next scan
                systick_wait_ms(500);

                // Do not move on the last iteration
                if(i != AMOUNT_OF_COLORS - 2) 
                {
                    drive_straight_distance(DISTANCE_BETWEEN_MEASUREMENTS);

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
