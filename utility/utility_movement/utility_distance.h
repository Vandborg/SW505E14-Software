#ifndef UTILITY_DISTANCE_H_   /* Include guard */
#define UTILITY_DISTANCE_H_

/* 
* Resets the driving motors count and thereby the distance driven
* Takes no parameters.
*/
void reset_distance(void);

/* 
* Returns the average distance of the two motors in cm.
* Takes no parameters.
* Returns an integer describing the distance in cm.
*/
int current_distance(void);


#endif
