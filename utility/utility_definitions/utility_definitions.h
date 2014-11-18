#ifndef UTILITY_DEFINITIONS_H_
    #define UTILITY_DEFINITIONS_H_

    // Boolean type
    #include "stdbool.h"

    // The first two numbers indicate which category the definition belongs to.  

    #define LCD_WIDTH   16
    #define LCD_HEIGHT  8

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
    #define SOUND_SUCCESS       150
    #define SOUND_INVALID_ID    151

    // Return codes for speed library
    #define SPEED_SUCCESS               160
    #define SPEED_CURRENT_BEFORE_START  161
    #define SPEED_CURRENT_AFTER_END     162
    #define SPEED_UNKNOW_ERROR          163

    // Motor port indentification
    #define RIGHT_MOTOR NXT_PORT_A
    #define LEFT_MOTOR  NXT_PORT_B
    #define FORK_MOTOR  NXT_PORT_C

    // Sensor port indentification
    #define COLOR_SENSOR        NXT_PORT_S3 // THIS SHOULD NOT BE USED... 
    #define COLOR_SENSOR_RIGHT  NXT_PORT_S2
    #define COLOR_SENSOR_LEFT   NXT_PORT_S1
    #define SONAR_SENSOR        NXT_PORT_S4 // THIS SHOULD NOT BE USED... 
    #define SONAR_SENSOR_FRONT  NXT_PORT_S3
    #define SONAR_SENSOR_REAR   NXT_PORT_S4

    // Time definitions
    #define ENTER_BUTTON_EXIT_TIMEOUT   2000

    // Braking definitions
    #define HIGH_BRAKEPOWER     2
    #define MEDIUM_BRAKEPOWER   1
    #define LOW_BRAKEPOWER      0
    #define BRAKE               1
    #define NO_BRAKE            0

    // Motor speeds
    #define MOTOR_NO_SPEED      0
    #define MOTOR_FULL_SPEED    100

    // Fork definitions
    #define MAXED_FORK      2
    #define LIFTED_FORK     1
    #define LOWERED_FORK    0

    // Defines used to turn wheels a certain fraction of a rotation without
    // having to compute the degrees on your own. Add more if needed
    #define WHOLE_ROTATION      1
    #define HALF_ROTATION       2
    #define THIRD_ROTATION      3
    #define QUARTER_ROTATION    4
    #define FIFTH_ROTATION      5
    #define SIXTH_ROTATION      6

    // Color definitions (for both left and right sensor)
    #define COLOR_RED_LEFT       0
    #define COLOR_RED_RIGHT      1
    #define COLOR_BLUE_LEFT      2
    #define COLOR_BLUE_RIGHT     3
    #define COLOR_BLACK_LEFT     4
    #define COLOR_BLACK_RIGHT    5
    #define COLOR_GRAY_LEFT      6
    #define COLOR_GRAY_RIGHT     7
    #define COLOR_WHITE_LEFT     8
    #define COLOR_WHITE_RIGHT    9 

    #define AMOUNT_OF_COLORS    10
    
    // Linefollow PID-controller definitions
    #define LINE_FOLLOW_SPEED       60

    #define KP                      21//35//27
    #define KI                       4.2//0// 2
    #define KD                      26.25//0//30

    #define INTEGRAL_MAX          2000     
    #define INTEGRAL_MIN -INTEGRAL_MAX      

    // Turn directions
    #define RIGHT_TURN          1
    #define LEFT_TURN           0 

    // Device statuses
    #define IDLE 'I'
    #define BUSY 'B'
    #define ERROR 'E'
    

#endif
