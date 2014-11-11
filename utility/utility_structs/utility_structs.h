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
        char directions[128];
        int next;
        int type_of_task;
    } navigation;

#endif
