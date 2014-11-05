// Third party libraries
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"
#include "stdlib.h"
#include "string.h"

// Own header
#include "utility/utility_bluetooth/utility_bluetooth.h"

// Own libraries
#include "utility/utility_lcd/utility_lcd.h"
#include "utility/utility_variables/utility_variables.h"
#include "utility/utility_string/utility_string.h"
#include "utility/utility_structs/utility_structs.h"


// Global variables to use
// color Color_left[AMOUNT_OF_COLORS];
// color Color_right[AMOUNT_OF_COLORS];
color Colors[AMOUNT_OF_COLORS];

DeclareTask(TASK_consume_bluetooth);

int send_package_bt(U8 package_type, char* package_data)
{
	// Calculate the size of package_data
    int package_data_size = strlen(package_data);

    // Calculate the total package size, assuming the package data size and
    // start, end, and type byte.
    int package_total_size = package_data_size + 3;
    
    // Check whether total data size is bigger than 256 bytes.
    if(package_total_size > 256)
    {
        // TODO: Send an error code for too big data array
        return -1;  
    }

    // Make an array that is as large as the total data size of the package.
    U8 package_to_send[package_total_size];

    // Putting the start, end, and type byte in the array.
    package_to_send[0] = START_BYTE;
	package_to_send[1] = package_type;
    package_to_send[package_total_size-1] = END_BYTE;

	// Fill the package data into the array at the correct array index
	for(int i = 0; i < package_data_size; i++)
	{
		package_to_send[i+2] = package_data[i];
	}

	// Send the request over BT
	ecrobot_send_bt(package_to_send, 0 , package_total_size);

    // Wait to ensure that continous requests are possible
	systick_wait_ms(1);

    return 1;
}

int read_bt_buffer(char* returnbuffer)
{
    // Read from bluetooth and write it to the buffer array.
    int bytes_read = (int)ecrobot_read_bt(returnbuffer, 0, 128);
    systick_wait_ms(5);
    return bytes_read;   
}

void update_color_bt(int color_id)
{
    // Tell the pc you want to update your color
    char color_id_str[4];
    lcd_display_line(LCD_LINE_ONE, int_to_string(color_id, color_id_str), true);
    systick_wait_ms(5000);

    send_package_bt(TYPE_UPDATE_COLOR, int_to_string(color_id, color_id_str));

    // Create buffer array with 128 bytes of space
    char data[128] = {0};

    // Get the current system time
    U32 start_time = ecrobot_get_systick_ms();
    
    // Read the BT buffer to verify that the that the returned BT data is 12
    // bytes (because a color bt packet is 12 bytes: Start, type, 3x colors of 
    // 3 byte, end byte). 
    while(read_bt_buffer(data) != 12) 
    { 
        // If more than 1000 seconds have passed and we havent exited the bt 
        // buffer read, send another bluetooth request and wait for it to return
        // once it returns, return from our own call. 
        if (ecrobot_get_systick_ms() - start_time > 1000)
        {
            update_color_bt(color_id);    
            return;
        }
    }

    // Check that the package type is of the SEND_COLOR type.
    if(data[1] == TYPE_SEND_COLOR)
    {
        // Compute the returned r, g and b values
        int r = char_to_int(data[2])*100 +
                char_to_int(data[3])*10 + 
                char_to_int(data[4]);

        int g = char_to_int(data[5])*100 + 
                char_to_int(data[6])*10 + 
                char_to_int(data[7]);

        int b = char_to_int(data[8])*100 + 
                char_to_int(data[9])*10 + 
                char_to_int(data[10]);

        // Update the correct color
        Colors[color_id].red = r;
        Colors[color_id].green = g;
        Colors[color_id].blue = b;
    }
}

void save_color_bt(int color_id,
                   int red, 
                   int green, 
                   int blue)
{

    char color_id_str[4];
    char red_str[4];
    char green_str[4];
    char blue_str[4];

    int_to_string(color_id, color_id_str);
    int_to_string(red, red_str);
    int_to_string(green, green_str);
    int_to_string(blue, blue_str);
    
    char color_id_str_with_pad[4] = {'0', '0', '0'};
    int missing_zeros = 3 - strlen(color_id_str);
    strcpy(color_id_str_with_pad + missing_zeros, color_id_str);

    char red_str_with_pad[4] = {'0', '0', '0'};
    missing_zeros = 3 - strlen(red_str);
    strcpy(red_str_with_pad + missing_zeros, red_str);

    char green_str_with_pad[4] = {'0', '0', '0'};
    missing_zeros = 3 - strlen(green_str);
    strcpy(green_str_with_pad + missing_zeros, green_str);

    char blue_str_with_pad[4] = {'0', '0', '0'};
    missing_zeros = 3 - strlen(blue_str);
    strcpy(blue_str_with_pad + missing_zeros, blue_str);
    
    char RGB_color_data[13] = "";
    strcpy(RGB_color_data, color_id_str_with_pad);
    strcat(RGB_color_data, red_str_with_pad);
    strcat(RGB_color_data, green_str_with_pad);
    strcat(RGB_color_data, blue_str_with_pad);

    send_package_bt(TYPE_SAVE_COLOR, RGB_color_data);
}

TASK(TASK_consume_bluetooth) 
{   
    TerminateTask();
}
