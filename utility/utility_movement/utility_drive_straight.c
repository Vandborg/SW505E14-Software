// Third party libraries
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"

// Own libraries
#include "../utility_sound/utility_sound.h"
#include "../utility_string/utility_string.h"
#include "../utility_definitions/utility_definitions.h"
#include "../utility_lcd/utility_lcd.h"
#include "utility_braking.h"
#include "utility_distance.h"

#define SPEED 100
#define DISPLACED_STARTUP 60


void drive_forward()
{
    nxt_motor_set_speed(RIGHT_MOTOR, DISPLACED_STARTUP, FALSE);
    nxt_motor_set_speed(LEFT_MOTOR, SPEED, TRUE);
    nxt_motor_set_speed(RIGHT_MOTOR, SPEED, TRUE);
}


