#ifndef UTILITY_BLUETOOTH_H_   /* Include guard */
    #define UTILITY_BLUETOOTH_H_

    // Delimiter bytes
    #define START_BYTE 0x2
    #define END_BYTE 0x3

    // Type bytes
    #define TYPE_UPDATE_COLOR 0x41
    #define TYPE_SAVE_COLOR 0x42
    #define TYPE_REPORT_OBSTACLE 0x43
    #define TYPE_UPDATE_STATUS 0x44
    #define TYPE_FETCH_PALLET 0x45
    #define TYPE_DELIVER_PALLET 0x46
    #define TYPE_FETCHED_COLOR 0x47

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
     * Function to receive data from the computer. Returns an int defining the
     * received amount of bytes. Max size possible to receive is 128 bytes. 
     */
    int read_bt_buffer(char* returnbuffer);

    /*
     * Update the global variable color with the given id
     * color_id     Identifier for a color
     */ 
    void update_color_bt(int color_id);

    /*
     * Saves the color on the PC using BT
     * color_id     Identifier for a color
     * red          Red-value of the color
     * green        Green-value of the color
     * blue         Blue-value of the color
     */
    void save_color_bt(int color_id,
                   int red, 
                   int green, 
                   int blue);
   

#endif
