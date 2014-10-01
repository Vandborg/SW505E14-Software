#ifndef UTILITY_LCD_H_   /* Include guard */
#define UTILITY_LCD_H_

int lcd_display_line(int line_number, char* string, int update_display);

void lcd_clear_line(int line_number, int update_display);

#endif
