// Third party libraries
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"

// Own header
#include "utility_line_follow.h"

// Own libraries
#include "utility/utility_lcd/utility_lcd.h"
#include "utility/utility_sound/utility_sound.h"
#include "utility/utility_structs/utility_structs.h"


#define COLOR_THRESHOLD 20

// Used to navigate
navigation Navigation;

// The status of the NXT
char Status;

DeclareTask(TASK_line_follow);
DeclareTask(TASK_color_scan);
DeclareAlarm(cyclic_alarm);
DeclareAlarm(cyclic_alarm_2);

// Prototypes
int get_light_level(U8 sensor);
void turn_direction(U8 direction);
void switch_sensors(void);
void cross_intersection(int line_follow_timeout);
bool is_red_color_colorsensor(void);
void line_recover(void);

// Persistent variables for PID, and color_scan/swap
U8 color_sensor = COLOR_SENSOR_LEFT;
U8 light_sensor = COLOR_SENSOR_RIGHT;

int offset_left = (416+140)/2;
int offset_right = (513+220)/2;

int integral = 0;
int lastError = 0;

bool line_follow = true;

S16 rgb[3];

// The global pereption of colors
color Colors[AMOUNT_OF_COLORS]; 

// Used to store the output from the color_scanner in the color_scan task
S16 color_scan_rgb[3] = {-1,-1,-1};

// Indidcates wether the NXT is on an edge or inside a node (intersection)
bool on_edge = true; 

void start_line_following(void)
{
    GetResource(RES_SCHEDULER);

    ecrobot_set_nxtcolorsensor(color_sensor, NXT_COLORSENSOR);
    ecrobot_set_nxtcolorsensor(light_sensor, NXT_COLORSENSOR);
    ecrobot_process_bg_nxtcolorsensor();

    SetRelAlarm(cyclic_alarm, 1, 50);
    SetRelAlarm(cyclic_alarm_2, 1, 300);

    ReleaseResource(RES_SCHEDULER);
}

void stop_line_following(void)
{
    GetResource(RES_SCHEDULER);

    nxt_motor_set_speed(RIGHT_MOTOR, 0, 1);
    nxt_motor_set_speed(LEFT_MOTOR, 0, 1);

    CancelAlarm(cyclic_alarm);
    CancelAlarm(cyclic_alarm_2);
    
    ecrobot_set_nxtcolorsensor(color_sensor, NXT_LIGHTSENSOR_NONE);
    ecrobot_set_nxtcolorsensor(light_sensor, NXT_LIGHTSENSOR_NONE);
    ecrobot_process_bg_nxtcolorsensor();

    ReleaseResource(RES_SCHEDULER);
}

TASK(TASK_line_follow)
{   
    // lcd_display_line(LCD_LINE_ONE, "TASK_line_follow", true);
    if (line_follow)
    {
        int powerA = 0;
        int powerB = 0;

        int error = 0;
        int derivative = 0;
        int turn = 0;

        int lightLevel = get_light_level(light_sensor);

        if (light_sensor == COLOR_SENSOR_LEFT)
        {
            error = lightLevel - offset_left; 
            integral = integral + error;
            derivative = error - lastError;

            turn = KP * error + KI * integral + KD * derivative;
            
            // This is needed because the k's are multiplied by a hundred
            turn = turn / 100; 

            powerA = LINE_FOLLOW_SPEED + turn;
            powerB = LINE_FOLLOW_SPEED - turn;
        }
        else
        {
            error = lightLevel - offset_right;
            integral = integral + error;
            derivative = error - lastError;
            
            turn = KP * error + KI * integral + KD * derivative;
            
            // This is needed because the k's are multiplied by a hundred
            turn = turn / 100; 

            powerA = LINE_FOLLOW_SPEED - turn;
            powerB = LINE_FOLLOW_SPEED + turn;    
        }

        nxt_motor_set_speed(RIGHT_MOTOR, powerA, 1);
        nxt_motor_set_speed(LEFT_MOTOR, powerB, 1);

        lastError = error;
    }

    TerminateTask();
}

bool is_red_color_colorsensor()
{
    // Update the color sensor
    // ecrobot_process_bg_nxtcolorsensor();

    // Read the color from the current color_sensor
    ecrobot_get_nxtcolorsensor_rgb(color_sensor, color_scan_rgb);

    // Declare the red color
    color red_color = {-1, -1, -1};

    // Store the correct red color
    if(color_sensor == COLOR_SENSOR_LEFT)
    {
        red_color = Colors[COLOR_RED_LEFT];
    }
    else if (color_sensor == COLOR_SENSOR_RIGHT)
    {
        red_color = Colors[COLOR_RED_RIGHT];
    }

    // Calculate differences from the measured scan and the percepted color
    int r_diff = red_color.red - color_scan_rgb[0];
    r_diff = r_diff > 0 ? r_diff : -r_diff;
    int g_diff = red_color.green - color_scan_rgb[1];
    g_diff = g_diff > 0 ? g_diff : -g_diff;
    int b_diff = red_color.blue - color_scan_rgb[2];
    b_diff = b_diff > 0 ? b_diff : -b_diff;

    // Check if the measured color is within 
    // acceptable threshold of the percepted color
    if(r_diff < COLOR_THRESHOLD && 
       g_diff < COLOR_THRESHOLD && 
       b_diff < COLOR_THRESHOLD)
    {
        return true;
    }

    return false;
}

TASK(TASK_color_scan)
{
    // lcd_display_line(LCD_LINE_ONE, "TASK_color_scan", true);
    if(is_red_color_colorsensor())
    {
        if(on_edge)
        {
            //char next_direction = Navigation.directions[Navigation.next];

            // TODO: Follow instruction
            switch('S')
            {
                case 'L':
                    if(light_sensor == COLOR_SENSOR_RIGHT)
                    {
                        // switch_sensors();
                    }
                    break;
                case 'R':
                    if(light_sensor == COLOR_SENSOR_LEFT)
                    {
                        // switch_sensors();
                    }
                    break;
                case 'S':
                    cross_intersection(1000);
                    break;
                default :
                    Status = ERROR;
            }
        }
        
        on_edge = !on_edge;
    }
    
    TerminateTask();
}

void turn_direction(U8 direction) 
{
    if((direction == RIGHT_TURN && light_sensor == COLOR_SENSOR_LEFT) ||
       (direction == LEFT_TURN && light_sensor == COLOR_SENSOR_RIGHT))
    {
        switch_sensors();
    }
}

int get_light_level(U8 sensor) 
{
    ecrobot_process_bg_nxtcolorsensor();

    S16 light_rgb[3];
    ecrobot_get_nxtcolorsensor_rgb(light_sensor, light_rgb);

    int light_level = (light_rgb[0] + light_rgb[1] + light_rgb[2]) / 3; 

    return light_level;
}

void switch_sensors(void)
{
    line_follow = false;
    U8 temp = color_sensor;
    color_sensor = light_sensor;
    light_sensor = temp;

    line_recover();
}

void cross_intersection(int line_follow_timeout)
{
    line_follow = false;
    int Kp = 10;
    int Ki = 1;
    int Kd = 12;

    int error = 0;
    int integral_straight = 0;
    int last_error = 0;
    int derivative = 0;

    int init_motor_count_left = nxt_motor_get_count(LEFT_MOTOR);
    int init_motor_count_right = nxt_motor_get_count(RIGHT_MOTOR);

    int current_count_left = 0;
    int current_count_right = 0;

    int output = 0;

    int powerA = 0;
    int powerB = 0;

    int time_stamp = systick_get_ms();

    while(systick_get_ms() - time_stamp < line_follow_timeout)
    {
        current_count_right = 
            nxt_motor_get_count(RIGHT_MOTOR) - init_motor_count_right;
        current_count_left = 
            nxt_motor_get_count(LEFT_MOTOR) - init_motor_count_left;

        error = current_count_right - current_count_left;
        integral_straight = (2/3) * integral * error;
        derivative = error - last_error;

        output = Kp * error + Ki * integral_straight + Kd * derivative;

        powerA = LINE_FOLLOW_SPEED - output;
        powerB = LINE_FOLLOW_SPEED + output;

        nxt_motor_set_speed(RIGHT_MOTOR, powerA, 1);
        nxt_motor_set_speed(LEFT_MOTOR, powerB, 1);
    }

    line_recover();
}

void line_recover(void) 
{
    line_follow = false;

    nxt_motor_set_speed(LEFT_MOTOR, 0, 1);
    nxt_motor_set_speed(RIGHT_MOTOR, 0, 1);

    int offset = 0;
    U8 light_motor = 0;
    U8 color_motor = 0;

    if (light_sensor == COLOR_SENSOR_LEFT)
    {
        offset = offset_left;
        light_motor = LEFT_MOTOR;
        color_motor = RIGHT_MOTOR;
    }
    else 
    {
        offset = offset_right;
        light_motor = RIGHT_MOTOR;
        color_motor = LEFT_MOTOR;
    }

    int light_level = get_light_level(light_sensor) - offset;

    int left_init_count = nxt_motor_get_count(LEFT_MOTOR);
    int right_init_count = nxt_motor_get_count(RIGHT_MOTOR);

    while(light_level <= -3 || light_level >= 3)
    {
        if(light_level < 0)
        {
            nxt_motor_set_speed(light_motor, 70, 0);
            nxt_motor_set_speed(color_motor, 0, 1);

        }
        else 
        {
            nxt_motor_set_speed(color_motor, 70, 0);
            nxt_motor_set_speed(light_motor, 0, 1);
        }
        light_level = get_light_level(light_sensor) - offset;
    }        

    int right_count = nxt_motor_get_count(RIGHT_MOTOR) - right_init_count;
    int left_count = nxt_motor_get_count(LEFT_MOTOR) - left_init_count;

    while(!(left_count >= right_count - 5 && 
          left_count <= right_count + 5))
    {
        if (left_count > right_count)
        {
            nxt_motor_set_speed(RIGHT_MOTOR, 70, 0);
            nxt_motor_set_speed(LEFT_MOTOR, 0, 1);            
        }
        else if(left_count < right_count)
        {
            nxt_motor_set_speed(LEFT_MOTOR, 70, 0);
            nxt_motor_set_speed(RIGHT_MOTOR, 0, 1);
        }

        right_count = nxt_motor_get_count(RIGHT_MOTOR) - right_init_count;
        left_count = nxt_motor_get_count(LEFT_MOTOR) - left_init_count;
    }

    integral = 0;
    lastError = 0;

    line_follow = true;
}
