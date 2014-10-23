#ifndef UTILITY_BLUETOOTH_H_   /* Include guard */
    #define UTILITY_BLUETOOTH_H_

    #define START_BYTE 0x2
    #define END_BYTE 0x3

    // Include definitions
    #include "utility/utility_definitions/utility_definitions.h"
    
    void send_startup_bt(void);

#endif
