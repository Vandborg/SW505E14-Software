#ifndef UTILITY_VARIABLES_H_
    #define UTILITY_VARIABLES_H_

    // Include definitions
    #include "utility/utility_definitions/utility_definitions.h"

    // Include structs
    #include "utility/utility_structs/utility_structs.h"

    extern color Colors[AMOUNT_OF_COLORS];
    extern navigation Navigation = {"", -1, 0};
    extern char Status = IDLE; 
    extern bool use_front_sonar_sensor = true;
    extern bool use_rear_sonar_sensor = false;

#endif
