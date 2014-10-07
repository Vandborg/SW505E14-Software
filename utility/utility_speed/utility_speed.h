#ifndef UTILITY_SPEED_H_
    #define UTILITY_SPEED_H_
    
    // Include definitions
    #include "utility/utility_definitions/utility_definitions.h"

    /*
     * Calculates a speed with ease in/out effect
     * start_mc     The motor count from where the easening should begin
     * end_mc       The motor count from where the easening should end
     * current_mc   The current motor count
     */
    int calculate_easing_speed(int start_mc, int end_mc, int current_mc);

#endif
