#ifndef FORK_RESET_H_   /* Include guard */
#define FORK_RESET_H_

/*
 *  Call this to start the menu that allows you to lower or lift the fork
 *  Returns nothing
 *  Takes no parameters
 */
void fork_reset(void);

/*
 * Function used to change the lifting display mode
 * Do not call this directly.
 * Returns nothing
 * Takes 1 input:
 * lifting_mode Int that describes whether the fork should lift or lower. 1 or 0
 */
void change_lifting_display_mode(int lifting_mode);

#endif
