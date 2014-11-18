#ifndef UTILITY_DISTANCE_H_   /* Include guard */
    #define UTILITY_DISTANCE_H_

    // Include definitions
    #include "utility/utility_definitions/utility_definitions.h"

    /* 
    * Resets the driving motors count and thereby the distance driven
    * Takes no parameters.
    */
    void reset_distance(void);

    /* 
    * Returns the average distance of the two motors in mm.
    * Takes no parameters.
    * Returns an integer describing the distance in mm.
    */
    int current_distance(void);

#endif
