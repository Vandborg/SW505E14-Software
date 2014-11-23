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


#define COLOR_THRESHOLD 50
// Used to navigate
navigation Navigation;

// The status of the NXT
char Status;
int drive_mode = NO_MODE;

DeclareTask(TASK_update_color_reg);
DeclareTask(TASK_drive_control);
DeclareTask(TASK_color_scan);
DeclareTask(TASK_check_navigation);
DeclareTask(TASK_cross_intersection);
DeclareAlarm(ALARM_drive_control);
DeclareAlarm(ALARM_color_scan);
DeclareAlarm(ALARM_update_color_reg);

// Prototypes
void start_line_following(void);
void stop_line_following(void);
bool is_red_color_colorsensor(void);
void turn_direction(U8 direction);
int get_light_level(U8 sensor);
void switch_sensors(void);
void cross_intersection(void);
void line_recover(void);
void line_following(void);
void swap(U8* a, U8* b);


// Persistent variables for PID, and color_scan/swap
U8 color_sensor = COLOR_SENSOR_LEFT;
U8 color_motor = COLOR_SENSOR_LEFT;
U8 light_sensor = COLOR_SENSOR_RIGHT;
U8 light_motor = COLOR_SENSOR_RIGHT;

int offset_left = (416+140)/2;
int offset_right = (513+220)/2;

int integral = 0;
int last_error = 0;

bool executing_task = false;

// The global pereption of colors
color Colors[AMOUNT_OF_COLORS]; 

// Used to store the output from the color_scanner in the color_scan task
S16 color_scan_rgb[3] = {-1,-1,-1};

// Indidcates wether the NXT is on an edge or inside a node (intersection)
bool on_edge = true; 
bool last_color_red = false;

// Indicates if the next has just come out of boot mode
bool first_time = true;

// Drive straight persistent variables
bool first_iteration = true; // Indicates if the init count should be saved
int integral_straight = 0;
int last_error_straight = 0;
int init_motor_count_left = 0;
int init_motor_count_right = 0;

TASK(TASK_update_color_reg)
{
    ecrobot_process_bg_nxtcolorsensor();
    TerminateTask();
}

TASK(TASK_color_scan)
{
    // There is some path left to follow
    if(Navigation.next > -1)
    {
        // Is the meassured color red and was the last color meassured not red
        if(is_red_color_colorsensor())
        {

            if(!last_color_red)
            {
                // Debugging sound
                play_sound(SOUND_TICK);

                // If the NXT is driving on an edge in the graph and not an vertex
                if(on_edge)
                {
                    // The next direction in the navigation
                    char next_direction = Navigation.directions[Navigation.next];

                    switch(next_direction)
                    {
                        case 'L':
                            turn_direction(LEFT_TURN);
                            break;
                        case 'R':
                            turn_direction(RIGHT_TURN);
                            break;
                        case 'S':
                            first_iteration = true;
                            drive_mode = CROSS_INTERSECTION;
                            break;
                        default :
                            Status = ERROR;
                    }
                }
                else
                {
                    drive_mode = LINE_FOLLOW;
                    Navigation.next = Navigation.next - 1;
                }
                on_edge = !on_edge;
            }
            
            // The color was red
            last_color_red = true;
        }
        else
        {
            // The color was not red
            last_color_red = false;
        }
    }
    else
    {
        drive_mode = NO_MODE;
        // TODO: Handle type of task (Fetch or deliver pallet)
        Status = IDLE;
    }
    
    TerminateTask();
}

TASK(TASK_drive_control)
{   
    switch(drive_mode)
    {
        case LINE_FOLLOW:
            line_following();
            break;
        case LINE_RECOVER:
            line_recover();
            break;
        case CROSS_INTERSECTION:
            cross_intersection();
            break;
        default:
            nxt_motor_set_speed(RIGHT_MOTOR, 0, 1);
            nxt_motor_set_speed(LEFT_MOTOR, 0, 1);
            break;
    }
    TerminateTask();
}

TASK(TASK_check_navigation)
{   
    if(Navigation.next > -1 && !executing_task)
    {
        start_line_following();
    }

    if (first_time)
    {
        SetRelAlarm(ALARM_drive_control, 1, 50);
        SetRelAlarm(ALARM_color_scan, 1, 100);
        first_time = false;
    }

    TerminateTask();
}

void cross_intersection(void)
{
    if(first_iteration) 
    {
        init_motor_count_right = nxt_motor_get_count(RIGHT_MOTOR);
        init_motor_count_left = nxt_motor_get_count(LEFT_MOTOR);
        first_iteration = false;
    }

    int current_count_left = 
        nxt_motor_get_count(LEFT_MOTOR) - init_motor_count_left;
    int current_count_right = 
        nxt_motor_get_count(RIGHT_MOTOR) - init_motor_count_right;

    int error_straight = current_count_right - current_count_left;

    integral_straight = (2/3) * integral_straight + error_straight;

    int derivative_straight = error_straight - last_error_straight;

    int output = 
        KP_STRAIGHT * error_straight + 
        KI_STRAIGHT * integral_straight + 
        KD_STRAIGHT * derivative_straight;

    int powerA = LINE_FOLLOW_SPEED - output;
    int powerB = LINE_FOLLOW_SPEED + output;

    nxt_motor_set_speed(RIGHT_MOTOR, powerA, 1);
    nxt_motor_set_speed(LEFT_MOTOR, powerB, 1);

    last_error_straight = error_straight;

    return;
}

void line_following(void) 
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

        if (integral > INTEGRAL_MAX)
        {
            integral = INTEGRAL_MAX;
        }
        else if(integral < INTEGRAL_MIN)
        {
            integral = INTEGRAL_MIN;
        }

        derivative = error - last_error;

        turn = 
            KP_LEFT * error + 
            KI_LEFT * integral + 
            KD_LEFT * derivative;
        
        // This is needed because the k's are multiplied by a hundred
        turn = turn / 100; 

        powerA = LINE_FOLLOW_SPEED + turn;
        powerB = LINE_FOLLOW_SPEED - turn;
    }
    else
    {
        error = lightLevel - offset_right;
        integral = integral + error;

        if (integral > INTEGRAL_MAX)
        {
            integral = INTEGRAL_MAX;
        }
        else if(integral < INTEGRAL_MIN)
        {
            integral = INTEGRAL_MIN;
        }

        derivative = error - last_error;
        
        turn = 
            KP_RIGHT * error + 
            KI_RIGHT * integral + 
            KD_RIGHT * derivative;
        
        // This is needed because the k's are multiplied by a hundred
        turn = turn / 100; 

        powerA = LINE_FOLLOW_SPEED - turn;
        powerB = LINE_FOLLOW_SPEED + turn;    
    }

    nxt_motor_set_speed(LEFT_MOTOR, powerA, 1);
    nxt_motor_set_speed(RIGHT_MOTOR, powerB, 1);

    last_error = error;

    return;
}

void start_line_following(void)
{
    GetResource(RES_SCHEDULER);

    executing_task = true;
    drive_mode = LINE_FOLLOW;

    ReleaseResource(RES_SCHEDULER);
}

void stop_line_following(void)
{
    GetResource(RES_SCHEDULER);

    executing_task = false;
    drive_mode = NO_MODE;

    ReleaseResource(RES_SCHEDULER);
}

bool is_red_color_colorsensor(void)
{
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
        // play_sound(SOUND_NOTIFICATION);
        return true;
    }

    return false;
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

    S16 light_rgb[3];
    ecrobot_get_nxtcolorsensor_rgb(light_sensor, light_rgb);

    int light_level = (light_rgb[0] + light_rgb[1] + light_rgb[2]) / 3; 

    return light_level;
}

void swap(U8* a, U8* b)
{
    U8 temp = *a;
    *a = *b;
    *b = temp;
}

void switch_sensors(void)
{
    drive_mode = NO_MODE;
    /*U8 temp = color_sensor;
    color_sensor = light_sensor;
    light_sensor = temp;*/

    swap(&color_sensor, &light_sensor);
    swap(&color_motor, &light_motor);

    first_iteration = true;
    drive_mode = LINE_RECOVER;
}

void line_recover(void)
{
    if (first_iteration)
    {
        init_motor_count_left = 
            nxt_motor_get_count(LEFT_MOTOR);
        init_motor_count_right = 
            nxt_motor_get_count(RIGHT_MOTOR);

        first_iteration = false;
    }

    int offset = light_motor == LEFT_MOTOR ? offset_left : offset_right;  
    int light_level = get_light_level(light_sensor) - offset;


    if(light_level <= -10 || light_level >= 10)
    {
        if(light_level > 0)
        {
            nxt_motor_set_speed(light_motor, 70, 0);
            nxt_motor_set_speed(color_motor, 0, 1);

        }
        else 
        {
            nxt_motor_set_speed(color_motor, 70, 0);
            nxt_motor_set_speed(light_motor, 0, 1);
        }
    }
    else 
    {
        int right_count = 
            nxt_motor_get_count(RIGHT_MOTOR) - init_motor_count_right;
        int left_count = 
            nxt_motor_get_count(LEFT_MOTOR) - init_motor_count_left;
        
        if(!(left_count >= right_count - 5 && 
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
        }
        else
        {
            drive_mode = LINE_FOLLOW;
        }
    }  
    return;        
}
