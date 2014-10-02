#ifndef UTILITY_LCD_H_
    #define UTILITY_LCD_H_
    
    /*
     * Will display a string on a specific line. Can update the display.
     * line_number_id   Identifier for the line number.
     * string           String to display on the line.
     * update_display   Indicate if the display should be updated. 
     */
    int lcd_display_line(int line_number_id, char* string, int update_display);
    
    /*
     * Will clear a specific line.
     * line_number_id   Identifier for the line number.
     * update_display   Indicate if the display should be updated. 
     */
    int lcd_clear_line(int line_number_id, int update_display);

    /*
     * Will display an array of strings form a specific line number for
     * a given number of lines.
     * start_line_number_id     Identifier for the line number to start.
     * number_of_lines          Number of lines to update the display.
     * display_content          Array of strings to write to the display.
     * update_display           Indicate if the display should be updated. 
     */
    int lcd_display_lines(int start_line_number_id, int number_of_lines, 
                          char display_content[8][17], int update_display);

#endif
