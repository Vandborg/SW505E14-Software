// Third party libraries
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"
#include "string.h"
 
// Own libraries
#include "boot_programs/boot_programs.h"
#include "utility/utility_definitions/utility_definitions.h"
#include "utility/utility_lcd/utility_lcd.h"
#include "utility/utility_sound/utility_sound.h"
#include "utility/utility_string/utility_string.h"
 
// System clock
void user_1ms_isr_type2(void) 
{ 
}
 
// On device initialization
void ecrobot_device_initialize(void)
{
    ecrobot_init_nxtcolorsensor(COLOR_SENSOR, NXT_COLORSENSOR);
    ecrobot_init_sonar_sensor(SONAR_SENSOR);
    ecrobot_init_bt_slave("password");
}
 
// On device termination
void ecrobot_device_terminate(void)
{
    ecrobot_term_nxtcolorsensor(COLOR_SENSOR);
    ecrobot_term_sonar_sensor(SONAR_SENSOR);
}
 
// The boot task of the program
TASK(TASK_boot) 
{   
    boot_device();
    
    static U8 start_up_signal[1] = {'r'};

    ecrobot_send_bt(start_up_signal, 0 , 1);



    static U8 black_color_red[4] = "123";
    while(1)
    {
        if(ecrobot_is_RUN_button_pressed())
        {
            ecrobot_send_bt(black_color_red, 0 , 3);
            lcd_display_line(LCD_LINE_ONE, "Data sent", true);
            break;
        }    

    }
    
 
    TerminateTask();
}
