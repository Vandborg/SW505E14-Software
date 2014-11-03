using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BTCom
{
    class BluetoothConnection : SerialPort
    {
        // Delimeter bytes
        private static byte START_BYTE = 0x2;
        private static byte END_BYTE = 0x3;
        
        // Type bytes
        private static byte TYPE_READ_COLOR = 0x41;
        private static byte TYPE_UPDATE_COLOR = 0x42;
        private static byte TYPE_REPORT_OBSTACLE = 0x43;
        private static byte TYPE_UPDATE_STATUS = 0x44;
        private static byte TYPE_FETCH_PALLET = 0x45;
        private static byte TYPE_DELIVER_PALLET = 0x46;
        private static byte TYPE_SEND_COLOR = 0x47;

        public BluetoothConnection(string portName)
            : base(portName)
        {
            this.Open();
            this.ReadTimeout = 10000;
            this.WriteTimeout = 10000;
        }

        public int SendPackageBT(byte package_type, char[] package_data)
        {
	        // Calculate the size of package_data
            int package_data_size = package_data.Length;

            // Calculate the total package size, assuming the package data size and
            // start, end, and type bytes.
            int package_total_size = (package_data_size + 3);
    
            // Check whether total data size is bigger than 256 bytes.
            if(package_total_size > 256)
            {
                // TODO: Send an error code for too big data array
                return 0;  
            }

            // Make an array that is as large as the total data size of the package.
            byte[] package_to_send = new byte[package_total_size];

            // Putting the start, end, and type byte in the array.
            package_to_send[0] = START_BYTE;
	        package_to_send[1] = package_type;
            package_to_send[package_total_size-1] = END_BYTE;

	        // Fill the package data into the array at the correct array index
	        for(int i = 0; i < package_data_size; i++)
	        {
		        package_to_send[i+2] = (byte) package_data[i];
	        }

	        // Send the request over BT
            this.Write(package_to_send, 0, package_total_size);

            // Wait to ensure that continous requests is possible
            System.Threading.Thread.Sleep(1);

            return 1;
        }

        public byte[] ReadPackageBT()
        {
            byte[] input = new byte[256];
            this.Read(input, 0, this.BytesToRead);

            return input;
        }

    }
}
