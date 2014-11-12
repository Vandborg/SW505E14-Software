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
color Colors[AMOUNT_OF_COLORS];
navigation Navigation;
char Status;

// Declaration of the task to consume bluetooth packages. 
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
        lcd_display_string_with_linesplit("Error: Array too big");
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

int read_buffer_bt(char* returnbuffer)
{
    // Read from bluetooth and write it to the buffer array.
    int bytes_read = (int)ecrobot_read_bt(returnbuffer, 0, 128);
    for (int i = 0; i < 128; ++i)
    {
        if (returnbuffer[i-1] == 0x03)
        {
            returnbuffer[i] = '\0';
            break;
        }
    }
    systick_wait_ms(5);
    return bytes_read;   
}

void update_color_bt(int color_id)
{
    // Tell the pc you want to update your color
    char color_id_str[4];

    send_package_bt(TYPE_UPDATE_COLOR, int_to_string(color_id, color_id_str));

    // Create buffer array with 128 bytes of space
    char data[128] = {0};

    // Get the current system time
    U32 start_time = ecrobot_get_systick_ms();
    
    // Read the BT buffer to verify that the that the returned BT data is 12
    // bytes (because a color bt package is 12 bytes: Start, type, 3x colors of 
    // 3 byte, end byte). 
    while(read_buffer_bt(data) != 12) 
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
    if(data[1] == TYPE_FETCHED_COLOR)
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

// Saves a color to the database
void save_color_bt(int color_id,
                   int red, 
                   int green, 
                   int blue)
{

    // Create 4 strings. Needed to construct the data package.
    char color_id_str[4];
    char red_str[4];
    char green_str[4];
    char blue_str[4];

    // Convert all int parameters to strings
    int_to_string(color_id, color_id_str);
    int_to_string(red, red_str);
    int_to_string(green, green_str);
    int_to_string(blue, blue_str);
    
    // Create an array filled with zeros. Compute how many zeros paddings are 
    // needed to create an int with 3 digits. Copy the correct string in with
    // the correct amount of zeros.
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
    
    // Put all four padded strings containing 3 digits into one string so we can
    // send it to the computer.
    char RGB_color_data[13] = "";
    strcpy(RGB_color_data, color_id_str_with_pad);
    strcat(RGB_color_data, red_str_with_pad);
    strcat(RGB_color_data, green_str_with_pad);
    strcat(RGB_color_data, blue_str_with_pad);

    // Send package.
    send_package_bt(TYPE_SAVE_COLOR, RGB_color_data);
}

void create_path_bt(char* package)
{
    // Compute how many directions we have. This is total length - start, type 
    // and end
    int direction_count = strlen(package)-3;

    // Fill the Navigation struct with the correct directions
    for (int i = 0; i < direction_count; ++i)
    {
        Navigation.directions[i] = package[i+2];
    }

    Navigation.directions[direction_count] = '\0';
    // Fill navigation with location of next direction and which type of task
    // we are currently doing. 
    Navigation.next = direction_count-1;
    Navigation.type_of_task = package[1];
    Status = BUSY;
    update_status_bt();
}

void update_status_bt(void)
{
    // Array to contain the status so it can be sent
    char status_str[2];
    
    // Put the status variable into the status array
    status_str[0] = Status;

    // Send update status package with the status to the pc.
    send_package_bt(TYPE_UPDATE_STATUS, status_str);
}



TASK(TASK_consume_bluetooth) 
{   
    // If the NXT is idling, tell the pc and get a new task. 
    if(Status == IDLE)
    {
        update_status_bt();

        // random wait for stuff to work
        systick_wait_ms(100);

        // Array containing new task from PC
        char new_task[128] = {0};
        if(read_buffer_bt(new_task) != 0)
        {
            // If a new task has been fetched, call function to compute path.
            create_path_bt(new_task);
        }
    }
    // If the status is error, tell the pc. 
    else if(Status == ERROR)
    {
        update_status_bt();
    }

    TerminateTask();
}
