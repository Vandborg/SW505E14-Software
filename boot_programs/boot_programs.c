// Third party libraries
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"
#include "stdio.h"
#include "string.h"

// Own header
#include "boot_programs/boot_programs.h"

// Own libraries
#include "utility_definitions/utility_definitions.h"
#include "utility_lcd/utility_lcd.h"
#include "utility_sound/utility_sound.h"

#define NUMBER_OF_MODES 10 // Number of modes including default
#define INITIALIZE_MODE -1 // First mode, used to initialize menu

// Prototypes
void select_mode(void);

// Struct to associate a name to a boot program
typedef struct 
{
    char* name;
    void (*func)(void);
} Boot_program;

// An array with space for ten NUMBER_OF_MODES boot programs
Boot_program boot_programs[NUMBER_OF_MODES];

// Used to display the currently selected boot program
int boot_program_counter = 0;

void add_boot_program(char* name, void (*func)(void)) 
{
    // Create struct based on arguments
    Boot_program new_element = 
    { 
        .name = name, 
        .func = func 
    };

    // Add the new element to the array
    boot_programs[boot_program_counter] = new_element;

    boot_program_counter++; // Increment the boot program counter
}

void boot_device(void)
{
    display_clear(TRUE); // Clear the display

    // Add all utility mode functions
    add_boot_program("COLOR SENSOR", &scan_colors);
    add_boot_program("SONAR SENSOR", &sonar_scan);

    // Wait until the user releases the finger from the run button
    while(ecrobot_is_RUN_button_pressed()) 
    {
        // Wait
    }

    // Show boot menu
    lcd_display_line(LCD_LINE_ONE,   "----- BOOT -----", FALSE);
    lcd_display_line(LCD_LINE_TWO,   "================", FALSE);

    lcd_display_line(LCD_LINE_FIVE,  "[              ]", FALSE);

    lcd_display_line(LCD_LINE_SEVEN, "================", FALSE);
    lcd_display_line(LCD_LINE_EIGHT, "      STRT  MODE", TRUE);

    // Setup for integers used to handle timing of the boot menu
    int current_tick = systick_get_ms();
    int next_tick = current_tick;
    int tick_delay = 500;

    int current_ticks = 0;
    int target_ticks = 14;

    // Give the user some time to interact with the unit
    while(current_ticks <= target_ticks) 
    {
        current_tick = systick_get_ms(); // Update the current tick

        // Check if the unit should tick
        if(next_tick <= current_tick) 
        {
            // Play tick sound every second tick
            if(current_ticks % 2 == 0) 
            {
                play_sound(SOUND_TICK);
            }

            // Update loading bar
            display_goto_xy(current_ticks + 1, 4);
            display_string("#");
            display_update();

            // Set next tick
            next_tick = next_tick + tick_delay;
            current_ticks++;
        }

        // Check if the run button is pressed
        if(ecrobot_is_RUN_button_pressed()) 
        {
            display_clear(1); // Clear the display
            select_mode();  // Go to the select mode menu
            break; // Exit from the loop
        }

        // Check if the enter button is pressed
        if(ecrobot_is_ENTER_button_pressed()) 
        {
            break; // Exit from the loop
        }
    }

    play_sound(SOUND_BUTTON_FEEDBACK); // Give button feedback
    display_clear(TRUE); // Clear the display
    play_sound(SOUND_MODE_START); // Return to the default program
}

void select_mode(void)
{
    // Wait until the user releases the finger from either buttons
    while(ecrobot_is_RUN_button_pressed() 
          || ecrobot_is_ENTER_button_pressed()) 
    {
        ; // Busy wait
    }

    // Return if there are not any boot programs
    if(boot_program_counter == 0) 
    {
        return;
    }

    // Used to check if the user released the buttons before new checks
    int run_button_released = TRUE;
    int enter_button_released = TRUE;

    /* 
     * Indicates which mode is currently being displayed to the user.
     * Initialised to be -1, which will force the program to simulate
     * the first button press and update the display correctly.
     */ 
    int mode = INITIALIZE_MODE;

    int exit_mode = 0; // An additional mode that is used to exit
    int line = 3; // Line number to display the selector on

    // Loop indefinitely until the user breaks
    while(TRUE) 
    {
        // Check if the run button is pressed
        if((ecrobot_is_RUN_button_pressed() && run_button_released) 
           || mode == INITIALIZE_MODE) 
        {
            run_button_released = FALSE; // The button was pressed
            play_sound(SOUND_BUTTON_FEEDBACK); // Play button click sound

            // Display the menu on first iteration
            if(mode == INITIALIZE_MODE) 
            {
                lcd_display_line(LCD_LINE_ONE,   "----- MENU -----", FALSE);
                lcd_display_line(LCD_LINE_TWO,   "================", FALSE);
                lcd_display_line(LCD_LINE_SEVEN, "================", FALSE);
                lcd_display_line(LCD_LINE_EIGHT, "  /   SLCT  NEXT", TRUE);
            }

            // Increment mode correctly
            mode = (mode + 1) % (boot_program_counter + 1);

            // Update the display to show the currently selected program
            display_goto_xy(0, 7);
            display_int(mode, 2);
            display_goto_xy(3, 7);
            display_int(boot_program_counter, 2);
            display_update();

            // Update the display corresponding to the current mode
            if(mode == exit_mode) 
            {
                lcd_display_line(LCD_LINE_FOUR, "<EXIT>", TRUE);
            }
            else 
            {
                lcd_display_line(LCD_LINE_FOUR, boot_programs[mode - 1].name, 
                                 TRUE);
            }
        }
        else if(ecrobot_is_RUN_button_pressed() == FALSE) 
        {
            run_button_released = TRUE; // The button was released
        }

        // Check if the enter button is pressed
        if(ecrobot_is_ENTER_button_pressed() && enter_button_released) 
        {
            enter_button_released = FALSE; // Button is pressed
            play_sound(SOUND_BUTTON_FEEDBACK); // Play button click sound
            display_clear(TRUE); // Clear the display

            // Wait until the user releases the button
            while(ecrobot_is_ENTER_button_pressed()) 
            {
                ; // Busy wait
            }

            // Test if the program is in the exit mode, and exit if it is
            if(mode == exit_mode) 
            {
                break;
            }
            else 
            {
                display_clear(TRUE); // Clear the display
                boot_programs[mode - 1].func(); // Call the function
                mode = INITIALIZE_MODE; // Reset the mode 
                display_clear(TRUE); // Clear the display
            }
        }
        else if(ecrobot_is_ENTER_button_pressed() == FALSE) 
        {
            enter_button_released = TRUE; // Enter button released
        }
    }
}
