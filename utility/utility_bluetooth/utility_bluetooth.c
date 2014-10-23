// Third party libraries
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"
#include "string.h"

// Own header
#include "utility_bluetooth.h"

void send_startup_bt(void)
{
    static U8 write_buffer[5] = {START_BYTE, 'R', 'D', 'Y', END_BYTE};
    ecrobot_send_bt(write_buffer, 0 , 5);
}
