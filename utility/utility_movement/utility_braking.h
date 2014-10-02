#ifndef UTILITY_BRAKING_H_   /* Include guard */
#define UTILITY_BRAKING_H_

/* 
 * Main function that that brakes the forklift.
 * Call this to brake.
 * Returns nothing
 * Takes 3 parameters:
 * brakepower       The desired brake power. Ranges from 0 to 2, low to hard.
 * left_motor_speed The current speed of the left motor. Goes from -100 to 100
 * right_motor_speed The current speed of the right motor. Goes from -100 to 100
 */
void forklift_brake(int breakpower, 
                    int left_motor_speed, 
                    int right_motor_speed);

/* 
 * Function that sets both motors to 0 speed and applies brake.
 * Do not call this.
 * Returns nothing
 * Takes no parameters
 */
void fullstop_brake(void);

/* 
 * Function that reduces both motors' speed gradually from current speed to 0. 
 * Do not call this.
 * Returns nothing
 * Takes 3 parameters:
 * brakepower: The desired brake power. Ranges from 0 to 2, low to hard.
 * left_motor_speed The current speed of the left motor. Goes from -100 to 100
 * right_motor_speed The current speed of the right motor. Goes from -100 to 100
 */
void fading_brake(int breakpower, 
                  int left_motor_speed, 
                  int right_motor_speed);

/* 
 * Function that reduces both motors' speed slowly from current speed to 0.
 * Do not call this.
 * Returns nothing
 * Takes 3 parameters:
 * brakepower: The desired brake power. Ranges from 0 to 2, low to hard.
 * left_motor_speed The current speed of the left motor. Goes from -100 to 100
 * right_motor_speed The current speed of the right motor. Goes from -100 to 100
 */
void slow_brake(int breakpower, 
                int left_motor_speed, 
                int right_motor_speed);

#endif
