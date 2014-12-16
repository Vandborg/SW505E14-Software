// Third party libraries
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"

// Own libraries
#include "boot_programs/boot_programs.h"
#include "boot_programs/fork_reset/fork_reset.h"
#include "utility/utility_bluetooth/utility_bluetooth.h"
#include "utility/utility_definitions/utility_definitions.h"
#include "utility/utility_movement/utility_movement.h"
#include "utility/utility_sound/utility_sound.h"
#include "utility/utility_structs/utility_structs.h"
#include "utility/utility_lcd/utility_lcd.h"
#include "utility/utility_variables/utility_variables.h"


navigation Navigation;

DeclareCounter(SysTimerCnt);
DeclareTask(TASK_update_color_reg);
DeclareTask(TASK_motor_control);
DeclareTask(TASK_information_handling);
DeclareTask(TASK_check_navigation);
DeclareTask(TASK_consume_bluetooth);


// System clock
void user_1ms_isr_type2(void) 
{
    SignalCounter(SysTimerCnt);
}

// On device initialization
void ecrobot_device_initialize(void)
{
    ecrobot_init_nxtcolorsensor(COLOR_SENSOR_LEFT, NXT_COLORSENSOR);
    ecrobot_init_nxtcolorsensor(COLOR_SENSOR_RIGHT, NXT_COLORSENSOR);
    ecrobot_init_sonar_sensor(SONAR_SENSOR_FRONT);
    ecrobot_init_sonar_sensor(SONAR_SENSOR_REAR);
    ecrobot_init_bt_slave("password");
}

// On device termination
void ecrobot_device_terminate(void)
{
    ecrobot_term_nxtcolorsensor(COLOR_SENSOR_LEFT);
    ecrobot_term_nxtcolorsensor(COLOR_SENSOR_RIGHT);
    ecrobot_term_sonar_sensor(SONAR_SENSOR_FRONT);
    ecrobot_term_sonar_sensor(SONAR_SENSOR_REAR);
}

void ErrorHook(StatusType ercd)
{
    if (ercd == E_OS_LIMIT)
    {
        TaskType id;
        GetTaskID(&id);

        if(TASK_information_handling == id) 
        {
            lcd_display_line(LCD_LINE_ONE, "Scan fejl", true);
        }
        else if(TASK_update_color_reg) 
        {
            // lcd_display_line(LCD_LINE_TWO, "Update fejl", true);
        }
        else if(TASK_check_navigation) 
        {
            lcd_display_line(LCD_LINE_FOUR, "Check fejl", true);
        }
        else if(TASK_motor_control)
        {
            lcd_display_line(LCD_LINE_FIVE, "Line fejl", true);
        }
        else if(TASK_consume_bluetooth)
        {
            lcd_display_line(LCD_LINE_SIX, "BT fejl", true);
        }
    }
}


void initialize_colors(void) 
{
    update_color_bt(COLOR_RED_LEFT);
    update_color_bt(COLOR_RED_RIGHT);
    update_color_bt(COLOR_BLUE_LEFT);
    update_color_bt(COLOR_BLUE_RIGHT);
    update_color_bt(COLOR_BLACK_LEFT);
    update_color_bt(COLOR_BLACK_RIGHT);
    update_color_bt(COLOR_GRAY_LEFT);
    update_color_bt(COLOR_GRAY_RIGHT);
    update_color_bt(COLOR_WHITE_LEFT);
    update_color_bt(COLOR_WHITE_RIGHT);
}

// The boot task of the program
TASK(TASK_boot) 
{   
    initialize_colors();
    boot_device();
    fork_reset();
    Status = BUSY;
    Navigation.directions[0] = 'U';
    Navigation.next = 0;
    TerminateTask();
}


