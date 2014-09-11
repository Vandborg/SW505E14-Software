/* helloworld.c for TOPPERS/ATK(OSEK) */ 
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "math.h"
#include "utility_lcd/utility_lcd.h"

/* nxtOSEK hook to be invoked from an ISR in category 2 */
void user_1ms_isr_type2(void){ /* do nothing */ }

TASK(OSEK_Task_Background)
{ 
    while(1)
    {   
    
        display_goto_xy(0,0);
        display_int(foo(2), 3);
        display_update();
    }
    ecrobot_init_sonar_sensor(NXT_PORT_S2);
}
