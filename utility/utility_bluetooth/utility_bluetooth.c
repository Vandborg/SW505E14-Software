// Third party libraries
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"
#include "string.h"

// Own header
#include "utility_bluetooth.h"

#define POST_REQUEST 1002
#define GET_REQUEST  1003

#define NOTIFY_REQUEST_BYTE 'P'
#define POST_REQUEST_BYTE 'P'
#define GET_REQUEST_BYTE 'G'


void send_request_bt(U8 request[])
{
	// Create tmp request array to hold startbyte, request and endbyte 
	int request_size = sizeof(request)/sizeof(U8)+2;
	U8 request_tosend[request_size];

	request_tosend[request_size-1] = END_BYTE;

	// Fill the request array to send
	for(int i = 1; i < request_size - 1; i++)
	{
		request_tosend[i] = request[i-1];
	}

	// Send the request over BT
	ecrobot_send_bt(request_tosend, 0 , request_size);

	// Wait to ensure that continous requests is possible
	systick_wait_ms(1);
}

void send_startup_bt(void)
{
	U8 request[] = {'R','D','Y'};
	send_request_bt(request);
}

void send_hello_bt(void)
{
    static U8 write_buffer[] = {START_BYTE, 'O', 'O', 'O', END_BYTE};
    ecrobot_send_bt(write_buffer, 0 , sizeof(write_buffer)/sizeof(U8));
}

void send_store_color_bt(void)
{
    static U8 write_buffer[] = {START_BYTE, 'S', 'T', 'O', ':', 'C', 'O', 'L', 'O', 'R', ':', '1', '2', '3', ':', '4', '5', '6', ':', '7', '8', '9',  END_BYTE};
    ecrobot_send_bt(write_buffer, 0 , sizeof(write_buffer)/sizeof(U8));
}

void send_get_color_bt(void)
{
    static U8 write_buffer[] = {START_BYTE, 'G', 'E', 'T', ':', 'C', 'O', 'L', 'O', 'R',  END_BYTE};
    ecrobot_send_bt(write_buffer, 0 , sizeof(write_buffer)/sizeof(U8));
}
