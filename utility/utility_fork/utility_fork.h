#ifndef UTILITY_FORK_H_H   /* Include guard */
#define UTILITY_FORK_H_H

/*
 * Function that checks which lift height is desired and calls the
 * lifting function with a converted value. 
 * Call this to move the fork.
 * Returns nothing
 * Takes 1 parameter:
 * lifting_mode     int that ranges from 0 to 2. Defines low, medium or max hght
 */
void operate_fork(int lifting_mode);

/*
 * Function that either lifts or lowers the fork to a desired height.
 * Do not call this
 * Returns nothing
 * Takes 1 parameter:
 * lifting_height    int that specifies the desired height. 
 */
void move_fork(int lifting_height);

#endif
