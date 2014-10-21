// Third party libraries
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"

// Own libraries
#include "boot_programs/boot_programs.h"
#include "utility/utility_definitions/utility_definitions.h"
#include "utility/utility_movement/utility_movement.h"

// System clock
void user_1ms_isr_type2(void) 
{ 
}

// On device initialization
void ecrobot_device_initialize(void)
{
	ecrobot_init_nxtcolorsensor(COLOR_SENSOR_RIGHT);
    ecrobot_init_nxtcolorsensor(COLOR_SENSOR_LEFT);
	ecrobot_init_sonar_sensor(SONAR_SENSOR);
}

// On device termination
void ecrobot_device_terminate(void)
{
	ecrobot_term_nxtcolorsensor(COLOR_SENSOR_RIGHT);
    ecrobot_term_nxtcolorsensor(COLOR_SENSOR_LEFT);
	ecrobot_term_sonar_sensor(SONAR_SENSOR);
}

// The boot task of the program
TASK(TASK_boot) 
{   
    ecrobot_set_nxtcolorsensor(COLOR_SENSOR_RIGHT, LIGHTSENSOR_RED);

    // while(true) {
    //     int lightLevel = ecrobot_get_nxtcolorsensor_light(COLOR_SENSOR_RIGHT);

    //     display_clear(0);
    //     display_goto_xy(0,0);
    //     display_int(lightLevel, 10);
    //     display_update();
    // }
    int Kp = 30;
    int Ki = 0.001;
    int Kd = 10;
    int Tp = 60;
    int offset = 330; 

    int integral = 0;
    int error = 0;
    int lastError = 0;
    int derivative = 0;
    int turn = 0;

    int powerA = 0;
    int powerB = 0;

    

    while(true) 
    {
        // LOGIC TO CHECK WHICH SENSOR TO USE <-----------------
        int lightLevel = ecrobot_get_nxtcolorsensor_light(COLOR_SENSOR_RIGHT);

        //CALCULATE ERROR
        error = lightLevel - offset;
        
        integral = integral + error;

        derivative = error - lastError;

        turn = Kp * error + Ki * integral + Kd * derivative;
        turn = turn / 100; // ---> This is only needed if the k's 
                           //      are multiplied by a hundred
        powerA = Tp + turn;
        powerB = Tp - turn;

        // Make motors go... 
        nxt_motor_set_speed(LEFT_MOTOR, powerA, 1);
        nxt_motor_set_speed(RIGHT_MOTOR, powerB, 1);

        lastError = error;
    }

	TerminateTask();
}
