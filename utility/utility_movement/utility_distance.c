// Third party libraries
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"

#define WHEEL_CIRCUMFERENCE_MM 134



void reset_distance(void) 
{
	nxt_motor_set_count(NXT_PORT_A, 0);
	nxt_motor_set_count(NXT_PORT_B, 0);
}

int current_distance(void)
{
	int distance_left = 
		((nxt_motor_get_count(NXT_PORT_A) * WHEEL_CIRCUMFERENCE_MM) / 360)/10;
	int distance_right = 
		((nxt_motor_get_count(NXT_PORT_B) * WHEEL_CIRCUMFERENCE_MM) / 360)/10;

	int distance_average = (distance_left + distance_right) / 2;

	return distance_average;
}
