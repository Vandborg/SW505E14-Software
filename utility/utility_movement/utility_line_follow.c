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
#define MOTOR_COUNT_PER_DEGREE 3
#define MOTOR_INSECURITY 30
// Used to navigate
navigation Navigation;

// The status of the NXT
char Status;
int drive_mode = NO_MODE;

DeclareTask(TASK_update_color_reg);
DeclareTask(TASK_drive_control);
DeclareTask(TASK_color_scan);
DeclareTask(TASK_check_navigation);
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
void swap(U32* a, U32* b);
void turn_arround(void);

// Persistent variables for PID, and color_scan/swap
U8 color_sensor = COLOR_SENSOR_LEFT;
U8 light_sensor = COLOR_SENSOR_RIGHT;

U32 color_motor = LEFT_MOTOR;
U32 light_motor = RIGHT_MOTOR;

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


bool first_instruction = true;
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

                // The next direction in the navigation
                char next_direction = Navigation.directions[Navigation.next];

                switch(next_direction)
                {
                    case 'L':
                        turn_direction(LEFT_TURN);
                        drive_mode = LINE_RECOVER;
                        break;
                    case 'R':
                        turn_direction(RIGHT_TURN);
                        drive_mode = LINE_RECOVER;
                        break;
                    case 'S':
                        first_iteration = true;
                        drive_mode = CROSS_INTERSECTION;
                        break;
                    case 'N':
                        break;
                    default :
                        Status = ERROR;
                }

                Navigation.next = Navigation.next - 1;
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
        stop_line_following();
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
        case TURN_ARROUND:
            turn_arround();
            break;
        case NO_MODE:
        default:
            nxt_motor_set_speed(RIGHT_MOTOR, 0, 1);
            nxt_motor_set_speed(LEFT_MOTOR, 0, 1);
    }
    TerminateTask();
}

TASK(TASK_check_navigation)
{   
    if (first_time)
    {
        SetRelAlarm(ALARM_drive_control, 1, 50);
        SetRelAlarm(ALARM_color_scan, 1, 100);

        // Calculate the left offset by taking the average of white & black rgb
        int black_light_level_left = 
            (Colors[COLOR_BLACK_LEFT].red +
             Colors[COLOR_BLACK_LEFT].green + 
             Colors[COLOR_BLACK_LEFT].blue) / 3;

        int white_light_level_left = 
            (Colors[COLOR_WHITE_LEFT].red +
             Colors[COLOR_WHITE_LEFT].green + 
             Colors[COLOR_WHITE_LEFT].blue) / 3;

        offset_left = (white_light_level_left + black_light_level_left) / 2;


        // Calculate the left offset by taking the average of white & black rgb
        int black_light_level_right = 
            (Colors[COLOR_BLACK_RIGHT].red +
             Colors[COLOR_BLACK_RIGHT].green + 
             Colors[COLOR_BLACK_RIGHT].blue) / 3;

        int white_light_level_right = 
            (Colors[COLOR_WHITE_RIGHT].red +
             Colors[COLOR_WHITE_RIGHT].green + 
             Colors[COLOR_WHITE_RIGHT].blue) / 3;

        offset_right = (white_light_level_right + black_light_level_right) / 2;

        first_time = false;
    }

    if(Navigation.next > -1 && !executing_task)
    {
        char next_direction = Navigation.directions[Navigation.next];

        switch(next_direction)
        {
            case 'L':
                turn_direction(LEFT_TURN);
                drive_mode = LINE_RECOVER;
                Navigation.next -= 1;
                break;
            case 'R':
                turn_direction(RIGHT_TURN);
                drive_mode = LINE_RECOVER;
                Navigation.next -= 1;
                break;
            default:
                break;
        }

        start_line_following();
    }

    TerminateTask();
}

void turn_arround(void)
{
    if(first_iteration)
    {
        init_motor_count_right = nxt_motor_get_count(RIGHT_MOTOR);
        init_motor_count_left  = nxt_motor_get_count(LEFT_MOTOR);

        first_iteration = false;
    }

    // Calculate at which motor count the motors should stop at
    int degrees_on_wheel_right = 180 * 
                                 MOTOR_COUNT_PER_DEGREE + 
                                 init_motor_count_right;

    int degrees_on_wheel_left  = 180 * 
                                 MOTOR_COUNT_PER_DEGREE -
                                 MOTOR_INSECURITY +
                                 init_motor_count_left;

    if (nxt_motor_get_count(RIGHT_MOTOR) >= degrees_on_wheel_right + 5 ||
        nxt_motor_get_count(RIGHT_MOTOR) >= degrees_on_wheel_right - 5)
    {
        nxt_motor_set_speed(RIGHT_MOTOR, 60, 1);
    }
    else 
    {
        nxt_motor_set_speed(RIGHT_MOTOR, 0, 1);
    }

    if (nxt_motor_get_count(LEFT_MOTOR) >= degrees_on_wheel_left + 5 ||
        nxt_motor_get_count(LEFT_MOTOR) >= degrees_on_wheel_left - 5)
    {
        nxt_motor_set_speed(LEFT_MOTOR, 60, 1);
    }
    else 
    {
        nxt_motor_set_speed(RIGHT_MOTOR, 0, 1);
    }

    if((nxt_motor_get_count(RIGHT_MOTOR) >= degrees_on_wheel_right + 5  ||
        nxt_motor_get_count(RIGHT_MOTOR) >= degrees_on_wheel_right - 5) &&
       (nxt_motor_get_count(LEFT_MOTOR) >= degrees_on_wheel_left + 5    ||
        nxt_motor_get_count(LEFT_MOTOR) >= degrees_on_wheel_left - 5))
    {
        drive_mode = NO_MODE;
    }

    return;
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
    first_iteration = true;
    drive_mode = LINE_RECOVER;

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

void emergency_stop(void) 
{
    drive_mode = NO_MODE;

    return;
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

void swap(U32* a, U32* b)
{
    U32 temp = *a;
    *a = *b;
    *b = temp;
}

void switch_sensors(void)
{
    drive_mode = NO_MODE;

    swap(&color_sensor, &light_sensor);
    swap(&color_motor, &light_motor);

    if(light_sensor == COLOR_SENSOR_LEFT)
    {
        lcd_display_line(LCD_LINE_FOUR, "LC", 1);
    }
    else
    {
        lcd_display_line(LCD_LINE_FOUR, "RC", 1);
    }
    if(light_motor == LEFT_MOTOR)
    {
        lcd_display_line(LCD_LINE_FIVE, "LM", 1);
    }
    else
    {
        lcd_display_line(LCD_LINE_FIVE, "RM", 1);
    }
}

bool line_found = false;

void line_recover(void)
{
    if (first_iteration)
    {
        init_motor_count_left = nxt_motor_get_count(LEFT_MOTOR);
        init_motor_count_right = nxt_motor_get_count(RIGHT_MOTOR);

        line_found = false;
        first_iteration = false;
    }

    // Set offset to be the offset corresponding to the left motor and sensor
    int offset = light_motor == LEFT_MOTOR ? offset_left : offset_right;  

    int light_level = get_light_level(light_sensor) - offset;


    if((light_level <= -10 || light_level >= 10) && 
        !line_found)
    {
        if(light_level < 0)
        {
            nxt_motor_set_speed(light_motor, 0, 1);
            nxt_motor_set_speed(color_motor, 70, 0);

        }
        else 
        {
            nxt_motor_set_speed(light_motor, 70, 0);
            nxt_motor_set_speed(color_motor, 0, 1);
        }
    }
    else 
    {
        line_found = true;

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
