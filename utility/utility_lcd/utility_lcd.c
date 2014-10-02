// Third party libraries
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "string.h"

// Own header
#include "utility/utility_lcd/utility_lcd.h"

// Own libraries
#include "utility/utility_definitions/utility_definitions.h"

#define LCD_MIN_LINE 0 // Minimum line number to write to
#define LCD_MAX_LINE 7 // Maximum line number to write to
#define LCD_MAX_NUMBER_OF_LINES 8 // Maximum amount of lines to write
#define LCD_MIN_NUMBER_OF_LINES 1 // Minimum amount of lines to write
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
        case LCD_LINE_SIX:
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

int convert_line_number_to_line_number_id(int line_number) 
{
    int line_number_id = LCD_INVALID_LINE_NUMBER;

    switch(line_number)
    {
        case 0: 
            line_number_id = LCD_LINE_ONE;
            break;
        case 1:
            line_number_id = LCD_LINE_TWO;
            break;
        case 2:
            line_number_id = LCD_LINE_THREE;
            break;
        case 3:
            line_number_id = LCD_LINE_FOUR;
            break;
        case 4:
            line_number_id = LCD_LINE_FIVE;
            break;
        case 5:
            line_number_id = LCD_LINE_SIX;
            break;
        case 6:
            line_number_id = LCD_LINE_SEVEN;
            break;
        case 7:
            line_number_id = LCD_LINE_EIGHT;
            break;
        default:
            line_number_id = LCD_INVALID_LINE_NUMBER;
            break;
    }

    return line_number_id;
}

int lcd_display_lines(int start_line_number_id, int number_of_lines, 
                      char display_content[8][17], int update_display)
{
    // Convert the line number identifier to normal line number
    int line_number = convert_line_id_to_line_number(start_line_number_id);

    // Check if the line number is invalid
    if(line_number == LCD_INVALID_LINE_NUMBER)
    {
        return LCD_INVALID_LINE_NUMBER;
    }

    // Check if the line number and number of lines is out of bounds
    if(number_of_lines > LCD_MAX_NUMBER_OF_LINES ||
       number_of_lines < LCD_MIN_NUMBER_OF_LINES ||
       line_number + number_of_lines > LCD_MAX_NUMBER_OF_LINES)
    {
        return LCD_LINES_OUT_OF_BOUNDS;
    }

    // Run through the display content and send it to the display buffer
    for(int i = 0; i < number_of_lines; i++)
    {
        lcd_display_line(convert_line_number_to_line_number_id(line_number + i),
                         display_content[i], FALSE);
    }

    // If the display should be updated update
    if(update_display)
    {
        display_update();
    }

    return LCD_SUCCESS; // Everything went well
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

    return LCD_SUCCESS; // Everything went well
}
