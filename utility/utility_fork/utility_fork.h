#ifndef UTILITY_FORK_H_H   /* Include guard */
    #define UTILITY_FORK_H_H

    // Include definitions
    #include "utility/utility_definitions/utility_definitions.h"

    /*
     * Function that checks which lift height is desired and calls the
     * lifting function with a converted value. 
     * Call this to move the fork.
     * Returns nothing
     * Takes 1 parameter:
     * lifting_mode     int that ranges from 0 to 2. Defines low, medium or max height
     */
    void operate_fork(int lifting_mode);

    /*
     * Place a pallet
     * Will place the pallet and move away from it
     * Returns nothing
     */
    void place_pallet();

    /*
     * Lift a pallet
     * Will lift the pallet
     * Returns nothing
     */
    void lift_pallet();

#endif
