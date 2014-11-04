﻿using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BTCom
{
    public class BluetoothConnection : SerialPort
    {
        // Delimeter bytes
        const byte START_BYTE = 0x2;
        const byte END_BYTE = 0x3;

        // Type bytes
        const byte TYPE_UPDATE_COLOR = 0x41;
        const byte TYPE_SAVE_COLOR = 0x42;
        const byte TYPE_REPORT_OBSTACLE = 0x43;
        const byte TYPE_UPDATE_STATUS = 0x44;
        const byte TYPE_FETCH_PALLET = 0x45;
        const byte TYPE_DELIVER_PALLET = 0x46;
        const byte TYPE_SEND_COLOR = 0x47;

        public BluetoothConnection(string portName)
            : base(portName)
        {
            // If the connection is not established try to connect.
            Console.WriteLine("[BT]: Attempting to connect.");
            while (!this.IsOpen)
            {
                try
                {
                    this.Open(); // Try to open the connection    
                }
                catch
                {
                    // Catched by retrying after sleep
                    Console.WriteLine("[BT]: Connection failed. Attempting to connect.");
                    System.Threading.Thread.Sleep(500);
                }
            }

            // Tell the user that the connection was established
            Console.WriteLine("[BT]: Connection established.");

            this.ReadTimeout = 10000; // Wait 10 sec before timeout on readbuffer
            this.WriteTimeout = 10000; // Wait 10 sec before timeout on writebuffer
        }

        // Consumes all packages on the buffer one at the time
        public void ConsumePackages()
        {
            byte[] btInputBuffer = ReadBTBuffer(); // The input on the buffer
            ConsumePackages(btInputBuffer); // Consume the buffer
        }

        // Consumes all packages on the buffer one at the time
        private void ConsumePackages(byte[] btInputBuffer)
        {
            int endIndexOfPackage = 0; // The index where the package ends

            // Run through the array of bytes on the buffer
            for (int i = 0; i < btInputBuffer.Length; i++)
            {
                if (btInputBuffer[i] == END_BYTE)
                {
                    byte[] package = new byte[i + 1]; // Allocate space for a pacakge
                    Array.Copy(btInputBuffer, package, i + 1); // Copies a package out
                    ConsumePackage(package); // Consume a package
                    endIndexOfPackage = i; // Sets the lates index to be the one from where we want to 
                    break;
                }
            }

            // If the package was not the last
            if (btInputBuffer.Length > endIndexOfPackage + 1)
            {
                // Allocate space for a new array to be splitted
                int newInputLength = btInputBuffer.Length - endIndexOfPackage;
                byte[] newBtInputBuffer = new byte[newInputLength];

                // Copies the rest of the array 
                Array.Copy(btInputBuffer, endIndexOfPackage + 1, newBtInputBuffer, 0, newInputLength);

                // Recursively splits this array
                ConsumePackages(newBtInputBuffer);
            }

        }

        // Sends a package with a given type
        public int SendPackageBT(byte package_type, byte[] package_data)
        {
            // Calculate the size of package_data
            int package_data_size = package_data.Length;

            // Calculate the total package size, assuming the package data size and
            // start, end, and type bytes.
            int package_total_size = (package_data_size + 3);

            // Check whether total data size is bigger than 256 bytes.
            if (package_total_size > 256)
            {
                // TODO: Send an error code for too big data array
                return 0;
            }

            // Make an array that is as large as the total data size of the package.
            byte[] package_to_send = new byte[package_total_size];

            // Putting the start, end, and type byte in the array.
            package_to_send[0] = START_BYTE;
            package_to_send[1] = package_type;
            package_to_send[package_total_size - 1] = END_BYTE;

            // Fill the package data into the array at the correct array index
            for (int i = 0; i < package_data_size; i++)
            {
                package_to_send[i + 2] = (byte)package_data[i];
            }

            // Send the request over BT
            this.Write(package_to_send, 0, package_total_size);

            // Wait to ensure that continous requests is possible
            System.Threading.Thread.Sleep(1);

            return 1;
        }

        // Reads the bytes on the buffer
        private byte[] ReadBTBuffer()
        {
            // The amount of bytes that is on the readbuffer
            int bytesToRead = this.BytesToRead;

            // Allocate array with enough space according to the amount of bytes
            byte[] input = new byte[bytesToRead];

            // Read from the buffer
            this.Read(input, 0, bytesToRead);

            // Return the byte array from the buffer
            return input;
        }

        // Consumes one package 
        private void ConsumePackage(byte[] package)
        {
            // Allocates data from the package
            byte[] data = new byte[package.Length - 3];

            // Finds the type of pakcage
            byte byteType = package[1];

            // Fills the data array with the corresponding data
            for (int i = 2, j = 0; i < package.Length - 1; i++, j++)
            {
                data[j] = package[i];
            }

            // Convert the data to a string 
            string dataString = Encoding.UTF8.GetString(data, 0, data.Length);

            // Find the correct method to consume the package
            switch (byteType)
            {
                case TYPE_UPDATE_COLOR:
                    /*  
                     *  Tell the user that the color is being fetched 
                     *  dataString contains the id of the color
                     */
                    Console.WriteLine("[BT]: Fetching color for \"" + dataString + "\"");

                    // Get the color from the database
                    Color requestedColor = Database.Instance.Data.Colors.FirstOrDefault(i => i.Value.Name == dataString).Value;

                    // Convert the RGB-values of the color to byte array
                    byte[] returnData = requestedColor.ToRGBBytes();

                    // Send the package back to the NXT
                    SendPackageBT(TYPE_SEND_COLOR, returnData);

                    break;
                case TYPE_SAVE_COLOR:
                    // TODO
                    break;
                case TYPE_REPORT_OBSTACLE:
                    // TODO
                    break;
                case TYPE_UPDATE_STATUS:
                    // TODO
                    break;
            }
            return;
        }
    }
}
