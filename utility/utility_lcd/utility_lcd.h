#ifndef UTILITY_LCD_H_
    #define UTILITY_LCD_H_

    // Include definitions
    #include "utility/utility_definitions/utility_definitions.h"
    
    /*
     * Will display a string on a specific line. Can update the display.
     * line_number_id   Identifier for the line number.
     * string           String to display on the line.
     * update_display   Indicate if the display should be updated. 
     */
    int lcd_display_line(int line_number_id, char* string, bool update_display);
    
    /*
     * Will clear a specific line.
     * line_number_id   Identifier for the line number.
     * update_display   Indicate if the display should be updated. 
     */
    int lcd_clear_line(int line_number_id, bool update_display);

    /*
     * Will display an array of strings form a specific line number for
     * a given number of lines.
     * start_line_number_id     Identifier for the line number to start.
     * number_of_lines          Number of lines to update the display.
     * display_content          Array of strings to write to the display.
     * update_display           Indicate if the display should be updated. 
     */
    int lcd_display_lines(int start_line_number_id, int number_of_lines, 
                          char display_content[8][17], bool update_display);

    /*
     * Will display a string on line at a given column.
     * line_number_id     Identifier for the line number.
     * column_number_id   Identifier for the column number.
     * string             String to be displayed.
     * clear_line         Indicate if the line should be cleared first.
     * update_display     Indicate if the display should be updated. . 
     */
    int lcd_display_string_at_column(int line_number_id, int column_number_id, 
                                   char* string, int clear_line, 
                                   bool update_display);

    /*
     * Will display a string on the required amount of lines. Automatically comp
     * utes which lines and columns to write at. Also clears the display before
     * writing anything. Returns nothing.
     * string             String to be displayed
     */
    void lcd_display_string_with_linesplit(char* string);

#endif

