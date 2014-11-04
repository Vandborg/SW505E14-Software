// Third party libraries
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "stdbool.h"
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

int convert_column_number_id_to_column_number(int column_number_id) 
{
    int column_number = LCD_INVALID_COLUMN_NUMBER;

    switch(column_number_id)
    {
        case LCD_COLUMN_ONE: 
            column_number = 0;
            break;
        case LCD_COLUMN_TWO:
            column_number = 1;
            break;
        case LCD_COLUMN_THREE:
            column_number = 2;
            break;
        case LCD_COLUMN_FOUR:
            column_number = 3;
            break;
        case LCD_COLUMN_FIVE:
            column_number = 4;
            break;
        case LCD_COLUMN_SIX:
            column_number = 5;
            break;
        case LCD_COLUMN_SEVEN:
            column_number = 6;
            break;
        case LCD_COLUMN_EIGHT:
            column_number = 7;
            break;
        case LCD_COLUMN_NINE:
            column_number = 8;
            break;
        case LCD_COLUMN_TEN: 
            column_number = 9;
            break;
        case LCD_COLUMN_ELEVEN:
            column_number = 10;
            break;
        case LCD_COLUMN_TWELVE:
            column_number = 11;
            break;
        case LCD_COLUMN_THIRTEEN:
            column_number = 12;
            break;
        case LCD_COLUMN_FOURTEEN:
            column_number = 13;
            break;
        case LCD_COLUMN_FIFTEEN:
            column_number = 14;
            break;
        case LCD_COLUMN_SIXTEEN:
            column_number = 15;
            break;
        default:
            column_number = LCD_INVALID_COLUMN_NUMBER;
            break;
    }

    return column_number;
}

int lcd_display_lines(int start_line_number_id, int number_of_lines, 
                      char display_content[8][17], bool update_display)
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

    int line_result = 0; // Placeholder for display line return code
    int new_lnid = 0; // Placedholder for line number id
    

    // Run through the display content and send it to the display buffer
    for(int i = 0; i < number_of_lines; i++)
    {
        new_lnid = convert_line_number_to_line_number_id(line_number + i);
        line_result = lcd_display_line(new_lnid, display_content[i], false);
        if(line_result != LCD_SUCCESS)
        {
            return line_result;
        }
    }

    // If the display should be updated update
    if(update_display)
    {
        display_update();
    }

    return LCD_SUCCESS; // Everything went well
}

int lcd_display_line(int line_number_id, char* string, bool update_display)
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
    int lcd_clear_line_result = lcd_clear_line(line_number_id, false);

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

int lcd_display_string_at_column(int line_number_id, int column_number_id, 
                                   char* string, int clear_line, 
                                   bool update_display)
{
    // Convert the line number identifier to normal line number
    int line_number = convert_line_id_to_line_number(line_number_id);

    // Check if the line number is invalid
    if(line_number == LCD_INVALID_LINE_NUMBER)
    {
        return LCD_INVALID_LINE_NUMBER;
    }

    // Convert the column number identifier to normal column number
    int column_number = convert_column_number_id_to_column_number(column_number_id);

    // Check if the column number is invalid
    if(column_number == LCD_INVALID_COLUMN_NUMBER)
    {
        return LCD_INVALID_COLUMN_NUMBER;
    }

    // Check if the string provided goes out of bounds
    if(strlen(string) + column_number > STR_MAX_LENGTH)
    {
        return LCD_STRING_OUT_OF_BOUNDS;
    }

    // Chcek if the line should be cleared first
    if(clear_line)
    {
        // Check if the clear line went well
        int clear_line_result = lcd_clear_line(line_number_id, false);
        if(clear_line_result != LCD_SUCCESS)
        {
            return clear_line_result;
        }
    }

    display_goto_xy(column_number, line_number); // Sets the cursor correctly
    display_string(string); // Puts the given string at cursor

    // Check if the display should be updated
    if(update_display) 
    {
        display_update();
    }

    return LCD_SUCCESS; // Everything went well
}

int lcd_clear_line(int line_number_id, bool update_display)
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

int lcd_display_string_with_linesplit(char* string)
{
    // Clear the display
    display_clear(false);

    // Compute the length of the received string
    int string_length = strlen(string);

    // Verify that the string is small enough to fit on the display.
    // If it isnt, return -1.
    if(string_length > (16*8))
    {
        return -1; 
    }

    // Loop that computes the line and column to write on. 
    // Writes one character at a time at the specified location.
    // Also clears the display before writing anything.
    for (int i = 0; i < string_length; ++i)
    {
        int line_to_write_on = i / 16;
        int column_to_write_on = i % 16;
        display_goto_xy(column_to_write_on, line_to_write_on);
        char print_string[2] = {string[i]};
        display_string(print_string);
    }

    display_update();
    return 1;
}
