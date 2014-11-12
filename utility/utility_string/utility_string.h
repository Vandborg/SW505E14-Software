#ifndef UTILITY_STRING_H_
    #define UTILITY_STRING_H_

    // Include definitions
    #include "utility/utility_definitions/utility_definitions.h"

    /*
     * Converts an integer to a string
     * number    The number to convert
     * sstring   Buffer string
     */
    char* int_to_string(int number, char string[]);

    /*
     * Converts an integer to a string
     * c    The char to convert to an int
     */
    int char_to_int(const char c);

#endif
