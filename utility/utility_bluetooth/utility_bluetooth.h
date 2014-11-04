#ifndef UTILITY_BLUETOOTH_H_   /* Include guard */
    #define UTILITY_BLUETOOTH_H_

    // Delimiter bytes
    #define START_BYTE 0x2
    #define END_BYTE 0x3

    // Type bytes
    #define TYPE_READ_COLOR 0x41
    #define TYPE_UPDATE_COLOR 0x42
    #define TYPE_REPORT_OBSTACLE 0x43
    #define TYPE_UPDATE_STATUS 0x44
    #define TYPE_FETCH_PALLET 0x45
    #define TYPE_DELIVER_PALLET 0x46
    #define TYPE_SEND_COLOR 0x47

    // Include definitions
    #include "utility/utility_definitions/utility_definitions.h"
    
    /*
     * Function to send data to the computer. Call with a type and the desired
     * string to send
     * package_type             Int that defines the type of the package    
     * package                  String that contains the package to send
     */
    int send_package_bt(U8 package_type, char* package);
    
    /*
     * Function to receive data from the computer. Returns a char array. Max
     * size possible to receive is 128 bytes. 
     */
    char* read_package_bt(void);
   

#endif
