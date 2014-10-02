// Third party libraries
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "string.h"

// Own header
#include "utility_lcd.h"

// Own libraries
#include "utility_definitions/utility_definitions.h"

#define LCD_MIN_LINE 0 // Minimum line number to write to
#define LCD_MAX_LINE 7 // Maximum line number to write to
#define STR_MAX_LENGTH 16 // Maximum string length to write
#define LEFTMOST_CHARACTER 0 // The index of the leftmost character
#define EMPTY_STRING "                " // Used to clear line

int convert_line_id_to_line_number(int line_number_id) 
{
    int line_number = LCD_INVALID_LINE_NUMBER;

    switch(line_number_id)
    {
        case LCD_LINE_ONE: 
            line_number = 0;
            break;
        case LCD_LINE_TWO:
            line_number = 1;
            break;
        case LCD_LINE_THREE:
            line_number = 2;
            break;
        case LCD_LINE_FOUR:
            line_number = 3;
            break;
        case LCD_LINE_FIVE:
            line_number = 4;
            break;
        case LCD_LINE_SIZ:
            line_number = 5;
            break;
        case LCD_LINE_SEVEN:
            line_number = 6;
            break;
        case LCD_LINE_EIGHT:
            line_number = 7;
            break;
        default:
            line_number = LCD_INVALID_LINE_NUMBER;
            break;
    }

    return line_number;
}

int lcd_display_line(int line_number_id, char* string, int update_display)
{
    // Convert the line number identifier to normal line number
    int line_number = convert_line_id_to_line_number(line_number_id);

    // Check if the line number is correct
    if(line_number == LCD_INVALID_LINE_NUMBER) 
    {
        return LCD_INVALID_LINE_NUMBER;
    }

    // Check if the string is too long
    if(strlen(string) > STR_MAX_LENGTH)
    {
        return LCD_INVALID_STRING_LENGTH;
    }

    // Clear the line
    int lcd_clear_line_result = lcd_clear_line(line_number_id, FALSE);

    // Check if the lcd clear line returns an error code
    if(lcd_clear_line_result == LCD_INVALID_LINE_NUMBER) 
    {
        return LCD_INVALID_LINE_NUMBER;
    } 

    // Update the correct buffer line of the display
    display_goto_xy(LEFTMOST_CHARACTER, line_number);
    display_string(string);

    // Check if the display should be updated
    if(update_display) 
    {
        display_update();
    }

    return LCD_SUCCESS; // Everything went well  
}

int lcd_clear_line(int line_number_id, int update_display)
{   
    // Convert the line number identifier to normal line number
    int line_number = convert_line_id_to_line_number(line_number_id);

    // Check if the line number identifier is valid
    if(line_number == LCD_INVALID_LINE_NUMBER) 
    {
        return LCD_INVALID_LINE_NUMBER;
    }

    // Write empty space to the line in order to clear it
    display_goto_xy(0, line_number);
    display_string(EMPTY_STRING);

    // Check if the display should be updated
    if(update_display) 
    {
        display_update();
    }
}
