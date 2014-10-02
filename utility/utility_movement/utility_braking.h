#ifndef UTILITY_BRAKING_H_   /* Include guard */
#define UTILITY_BRAKING_H_

/* 
 * Main function that that brakes the forklift.
 * Call this to brake.
 * Returns nothing
 * Takes 2 parameters:
 * brakepower       The desired brake power. Ranges from 0 to 2, low to hard.
 * forklift_speed   The current speed of the forklift. Ranges from -100 to 100.
 */
void forklift_brake(int breakpower, int forklift_speed);

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
 * Takes 2 parameters:
 * brakepower: The desired brake power. Ranges from 0 to 2, low to hard.
 * forklift_speed   The current speed of the forklift. Ranges from -100 to 100.
 */
void fading_brake(int breakpower, int forklift_speed);

/* 
 * Function that reduces both motors' speed slowly from current speed to 0.
 * Do not call this.
 * Returns nothing
 * Takes 2 parameters:
 * brakepower: The desired brake power. Ranges from 0 to 2, low to hard.
 * forklift_speed   The current speed of the forklift. Ranges from -100 to 100.
 */
void slow_brake(int breakpower, int forklift_speed);

#endif
