#ifndef UTILITY_DRIVE_STRAIGHT_H_   /* Include guard */
    #define UTILITY_DRIVE_STRAIGHT_H_

    /*
     * Makes the truck move forward without displaced start
     * Takes no parameters
     */
    void drive_forward(void);

    /*
     * Drives a specific distance forwards 
     * distance   Distance meassuared in motor count
     */
    void drive_straight_distance(int distance);


#endif
