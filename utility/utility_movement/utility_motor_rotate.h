#ifndef UTILITY_MOTOR_ROTATE_H_   /* Include guard */
#define UTILITY_MOTOR_ROTATE_H_

// Include definitions
#include "utility/utility_definitions/utility_definitions.h"

/*
 * Function that handles rotating a single motor a specific amount of degrees
 * with a specific speed
 * Returns nothing
 * Takes 3 parameters:
 * Degrees       The amount of degrees you want to rotate the motor
 * Speed         The speed you want to rotate the motor at
 * Motor         The name of the motor you want to turn. LEFT_MOTOR/RIGHT_MOTOR
 */
void motor_rotate_degrees(int degrees, int speed, U32 motor);




/*
 * Function that calls the degrees function, but takes an amount of rotations
 * as input instead. Can also use the mode variable to specify fraction turns.
 * Returns nothing
 * Takes 4 parameters:
 * Degrees       The amount of degrees you want to rotate the motor
 * Mode          The fraction type you want to use. whole, half, quarters etc.
 *               they are specified in utility_definitions.h.
 * Speed         The speed you want to rotate the motor at
 * Motor         The name of the motor you want to turn. LEFT_MOTOR/RIGHT_MOTOR
 */
void motor_rotate_rotations(int rotations, int mode, int speed, U32 motor);

#endif
