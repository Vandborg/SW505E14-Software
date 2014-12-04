// Third party libraries
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"

// Own header
#include "utility_fork.h"

// Own libraries
#include "utility/utility_definitions/utility_definitions.h"
#include "utility/utility_bluetooth/utility_bluetooth.h"
#include "utility/utility_variables/utility_variables.h"
#include "utility/utility_structs/utility_structs.h"
#include "utility/utility_lcd/utility_lcd.h"
#include "utility/utility_sound/utility_sound.h"

#define LIFTING_SPEED_UP 70      // This is an arbitrary value. Change if needed
#define LIFTING_SPEED_DOWN -55   // This is an arbitrary value. Change if needed
#define LIFTING_MODE_MAX 800     // This is an arbitrary value. Change if needed
#define LIFTING_MODE_MID 275     // This is an arbitrary value. Change if needed
#define LIFTING_MODE_LOW 35      // This is an arbitrary value. Change if needed

// Function that checks which lift height is desired and calls the lifting
// function with a converted value. Cases can be found in utility_definitions.h
// void operate_fork(int lifting_mode)
// {
//     switch(lifting_mode)
//     {
//         case MAXED_FORK:
//             move_fork(LIFTING_MODE_MAX);
//             break;
//         case LIFTED_FORK:
//             move_fork(LIFTING_MODE_MID);
//             break;
//         case LOWERED_FORK:
//             move_fork(LIFTING_MODE_LOW);
//             break;
//         default:
//             break;
//     }
// }

// //Function that moves the fork either up or down to a desired height
// void move_fork(int lifting_height)
// {
//     // Gets the current height of the fork. Assuming you calibrated
//     int current_height = nxt_motor_get_count(FORK_MOTOR);

//     // Checks if the desired height is higher than the current
//     if(lifting_height > current_height)
//     {
//         nxt_motor_set_speed(FORK_MOTOR, LIFTING_SPEED_UP, BRAKE);
//     }
//     // Checks if the desired height is lower than the current
//     else if(lifting_height < current_height)
//     {
//         nxt_motor_set_speed(FORK_MOTOR, LIFTING_SPEED_DOWN, NO_BRAKE);       
//     }
//     else
//     {
//         // Once the desired height has been reached, stop the motor.
//         nxt_motor_set_speed(FORK_MOTOR, MOTOR_NO_SPEED, BRAKE);
//     }

// }
