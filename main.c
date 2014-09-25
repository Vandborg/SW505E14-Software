#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "string.h"

// Own libraries
#include "utility_mode_handling/utility_mode_handling.h" 
#include "utility_sound/utility_sound.h"  
#include "color_scanning/color_scanning.h"

/* nxtOSEK hook to be invoked from an ISR in category 2 */
void user_1ms_isr_type2(void) { }

TASK(Task1) 
{ 
	scan_colors();
	TerminateTask();
}
