#ifndef UTILITY_LINE_FOLLOW_H_   
    #define UTILITY_LINE_FOLLOW_H_

    #include "utility/utility_definitions/utility_definitions.h"

    /* 
     * Function that starts the linefollowing tasks, and makes the necessary 
     * configurations to the device. This function locks the scheduler.
     * Returns nothing
     * Takes no parameters
     */
    void start_line_following(void);

    /* 
     * Function that stops the linefollowing tasks, turns off colorsensors 
     * and stops the motors. This function locks the scheduler.
     * Returns nothing
     * Takes no parameters
     */
    void stop_line_following(void);


    /* 
     * Function that stops PALL-E.
     * Returns nothing
     * Takes no parameters
     */
     void emergency_stop(void);
#endif

