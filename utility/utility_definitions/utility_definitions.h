#ifndef UTILITY_DEFINITIONS_H_
    #define UTILITY_DEFINITIONS_H_

    // The first two numbers indicate which category the definition belongs to.  

    // Boolean shortcuts
    #ifndef TRUE 
        #define TRUE 1 
    #endif

    #ifndef FALSE 
        #define FALSE 0 
    #endif

    // Sound indentification
    #define SOUND_STARTUP           110
    #define SOUND_SHUTDOWN          111
    #define SOUND_BUTTON_FEEDBACK   112
    #define SOUND_MODE_WAIT         113
    #define SOUND_MODE_SELECT       114
    #define SOUND_MODE_START        115
    #define SOUND_NOTIFICATION      116
    #define SOUND_TICK              117

    // Display line 
    #define LCD_LINE_ONE    120
    #define LCD_LINE_TWO    121
    #define LCD_LINE_THREE  122
    #define LCD_LINE_FOUR   123
    #define LCD_LINE_FIVE   124
    #define LCD_LINE_SIX    125
    #define LCD_LINE_SEVEN  126
    #define LCD_LINE_EIGHT  127

    // Return codes for lcd
    #define LCD_SUCCESS 130
    #define LCD_INVALID_LINE_NUMBER 131
    #define LCD_INVALID_STRING_LENGTH 132

    // Motor port indentification
    #define LEFT_MOTOR NXT_PORT_A
    #define RIGHT_MOTOR NXT_PORT_B
    #define FORK_MOTOR NXT_PORT_C

    // Sensor port indentification
    #define COLOR_SENSOR NXT_PORT_S1
    #define SONAR_SENSOR NXT_PORT_S2

    // Time definitions
    #define ENTER_BUTTON_EXIT_TIMEOUT 2000

#endif
