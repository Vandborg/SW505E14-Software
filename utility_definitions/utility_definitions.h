#ifndef UTILITY_DEFINITIONS_H_   /* Include guard */
#define UTILITY_DEFINITIONS_H_

// The first two numbers indicate which category the definition belongs to. ie 11 for sound 

// Sound indentification
#define SOUND_STARTUP 			110
#define SOUND_SHUTDOWN 			111
#define SOUND_BUTTON_FEEDBACK 	112
#define SOUND_MODE_WAIT 		113
#define SOUND_MODE_SELECT		114
#define SOUND_MODE_START 		115
#define SOUND_NOTIFICATION		116
#define SOUND_TICK				117

// Motor port indentification
#define LEFT_MOTOR NXT_PORT_A
#define RIGHT_MOTOR NXT_PORT_B

// Sensor port indentification
#define COLOR_SENSOR NXT_PORT_S1

// Time definitions
#define ENTER_BUTTON_EXIT_TIMEOUT 2000

#endif
