// Third party libraries
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"
#include "string.h"
 
// Own libraries
#include "boot_programs/boot_programs.h"
#include "utility/utility_definitions/utility_definitions.h"
#include "utility/utility_bluetooth/utility_bluetooth.h"
#include "utility/utility_lcd/utility_lcd.h"

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

    send_startup_bt();
    


    while(1)
    {
        if(ecrobot_is_RUN_button_pressed())
        {
            send_hello_bt();
            send_store_color_bt();
            lcd_display_line(LCD_LINE_ONE, "STO:COLOR", true);
            systick_wait_ms(1000);
        }
        if(ecrobot_is_ENTER_button_pressed())
        {
            send_hello_bt();
            send_get_color_bt();
            lcd_display_line(LCD_LINE_ONE, "GET:COLOR", true);
            systick_wait_ms(1000);
        }
    }
 
    TerminateTask();
}
