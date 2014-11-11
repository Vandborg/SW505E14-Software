#ifndef UTILITY_STRUCTS_H_
    #define UTILITY_STRUCTS_H_

    // Include definitions
    #include "utility/utility_definitions/utility_definitions.h"

    typedef struct 
    {
        int red;
        int green;
        int blue;
    } color;

    typedef struct
    {
        char* directions;
        int next;
    } navigation;

#endif
