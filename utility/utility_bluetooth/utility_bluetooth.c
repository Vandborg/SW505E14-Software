// Third party libraries
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"
#include "string.h"

// Own header
#include "utility_bluetooth.h"

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
	int bytes_sent = ecrobot_send_bt(package_to_send, 0 , package_total_size);

    // Wait to ensure that continous requests is possible
	systick_wait_ms(1);

    return 1;

}

char* read_package_bt(void)
{
    // Buffer char array to write in.
    char buffer[128];

    // Read from bluetooth and write it to the buffer array.
    int bytes_received = ecrobot_read_bt(buffer, 0, 128);

    // Create array that is only as big as the received data
    char returndata[bytes_received];

    // Copy the received data from the buffer array to the returndata array
    strcpy(returndata, buffer);

    return returndata;
}
