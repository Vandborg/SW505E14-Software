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

    #define LCD_WIDTH 16
    #define LCD_HEIGHT 8

    // Sound indentification
    #define SOUND_STARTUP           110
    #define SOUND_SHUTDOWN          111
    #define SOUND_BUTTON_FEEDBACK   112
    #define SOUND_MODE_WAIT         113
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
    #define LCD_SUCCESS                130
    #define LCD_INVALID_LINE_NUMBER    131    
    #define LCD_INVALID_STRING_LENGTH  132
    #define LCD_LINES_OUT_OF_BOUNDS    133
    #define LCD_INVALID_COLUMN_NUMBER  134
    #define LCD_STRING_OUT_OF_BOUNDS   135

    // Display column 
    #define LCD_COLUMN_ONE          140
    #define LCD_COLUMN_TWO          141
    #define LCD_COLUMN_THREE        142
    #define LCD_COLUMN_FOUR         143
    #define LCD_COLUMN_FIVE         144
    #define LCD_COLUMN_SIX          145
    #define LCD_COLUMN_SEVEN        146
    #define LCD_COLUMN_EIGHT        147
    #define LCD_COLUMN_NINE         148
    #define LCD_COLUMN_TEN          149
    #define LCD_COLUMN_ELEVEN       1410
    #define LCD_COLUMN_TWELVE       1411
    #define LCD_COLUMN_THIRTEEN     1412
    #define LCD_COLUMN_FOURTEEN     1413
    #define LCD_COLUMN_FIFTEEN      1414
    #define LCD_COLUMN_SIXTEEN      1415
    
    // Return codes for sound library
    #define SOUND_SUCCESS 150
    #define SOUND_INVALID_ID 151

    // Motor port indentification
    #define LEFT_MOTOR NXT_PORT_A
    #define RIGHT_MOTOR NXT_PORT_B
    #define FORK_MOTOR NXT_PORT_C

    // Sensor port indentification
    #define COLOR_SENSOR NXT_PORT_S1
    #define SONAR_SENSOR NXT_PORT_S2

    // Time definitions
    #define ENTER_BUTTON_EXIT_TIMEOUT 2000

    // Braking definitions
    #define HIGH_BRAKEPOWER 2
    #define MEDIUM_BRAKEPOWER 1
    #define LOW_BRAKEPOWER 0
    #define BRAKE 1
    #define NO_BRAKE 0

    // Motor speeds
    #define MOTOR_NO_SPEED 0
    #define MOTOR_FULL_SPEED 100

    // Fork definitions
    #define MAXED_FORK 2
    #define LIFTED_FORK 1
    #define LOWERED_FORK 0

    // Defines used to turn wheels a certain rotation without having to
    // compute the degrees on your own. Add more if needed
    #define WHOLE_ROTATION 1
    #define HALF_ROTATION 2
    #define THIRD_ROTATION 3
    #define QUARTER_ROTATION 4
    #define FIFTH_ROTATION 5
    #define SIXTH_ROTATION 6


#endif
