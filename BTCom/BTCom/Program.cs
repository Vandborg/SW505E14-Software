using SQLite;
using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


 
namespace BTCom
{   
    class Program
    {
        // Defines for BT-protocol
        public const byte START_BYTE = 0x2;
        public const byte END_BYTE = 0x3;

        // List of requests
        static private List<string> _Requests = new List<string>();

        static public List<string> Requests
        {
            get { return _Requests; }
            set { _Requests = value; }
        }

        // List of bytes that have been read and is ready to be 
        static private List<byte> _ReadBuffer = new List<byte>();

        static public List<byte> ReadBuffer
        {
            get { return _ReadBuffer; }
            set { _ReadBuffer = value; }
        }
        
	
        // Reads the chars on the buffer untill buffer is empty
        static private byte[] ReadBytesOnBuffer(SerialPort BluetoothConnection)
        {
            byte[] output = new byte[256];

            // DEBUG
            if (BluetoothConnection.BytesToRead > 0)
            {
                Console.WriteLine("BTR: " + BluetoothConnection.BytesToRead);
            }

            
            for (int i = 0; BluetoothConnection.BytesToRead > 0; i++ )
            {
                Console.WriteLine("I: " + i);
                output[i] = (byte) BluetoothConnection.ReadByte();
            }

            return output; 
        }

        static private void QueueRequestFromBuffer(SerialPort BluetoothConnection)
        {
            // Read the bytes of the buffer
            byte[] bytesOnBuffer = ReadBytesOnBuffer(BluetoothConnection);
            
            //Append the read bytes on the buffer
            ReadBuffer.AddRange(bytesOnBuffer.ToList());

            //Check if there exist a request that needs to be placed in the request list
            int indexOfStart = -1;
            int indexOfEnd = -1;
            for(int count = 0; ReadBuffer.Count > count; count++)
            {
                byte b = ReadBuffer[count];
                if (b == START_BYTE)
                {
                    indexOfStart = count;
                }
                else if (b == END_BYTE)
                {
                    indexOfEnd = count;
                }

                // There were found both a start and an end of a request
                if (indexOfStart != -1 && indexOfEnd != -1)
                {
                    // A request was found
                    if (indexOfStart < indexOfEnd)
                    {
                        // Fetch the request 
                        List<byte> requestBytes = ReadBuffer.GetRange(indexOfStart + 1, indexOfEnd - indexOfStart - 1);

                        // Add the request to the request list
                        Requests.Add(ConvertBytesRequestToString(requestBytes));

                        // Remove everything before the endbyte, including the endbyte
                        // [S][][][][][E][][][][]--> [][][][]
                        ReadBuffer.RemoveRange(0, indexOfEnd + 1);

                    }
                    // A request will never be found in the front of the list before indexOfStart
                    else if (indexOfEnd < indexOfStart)
                    {
                        // Remove everything from before the startbyte, excluding the startbyte
                        // [E][][][][][S][][][][] --> [S][][][][]
                        ReadBuffer.RemoveRange(0, indexOfStart);
                    }
                    else
                    {
                        // The byte was both START_BYTE and END_BYTE something went wrong
                        throw new Exception("Data on ReadBuffer is corrupted!");
                    }
                }
            }

        }

        static private string ConvertBytesRequestToString(List<byte> bytes)
        {
            // Placeholder for the request string
            char[] requestCharArray = new char[bytes.Count];

            // Go through all bytes in the request and convert them to chars
            int count = 0;
            foreach (byte b in bytes)
            {
                requestCharArray[count] = (char) b;
                count++;
            }

            // Return the converted the char array of chars to a strin
            return new string(requestCharArray);
        }

        static void Main(string[] args)
        {
            // Opening database-connection
            Console.WriteLine("Opening DB-connection");
            var db = new SQLiteConnection("DataBase");
            Console.WriteLine("Connected to DataBase");
            db.CreateTable<Setting>();

            // Setup the BluetoothConnection
            SerialPort BluetoothConnection = new SerialPort("COM5");
            BluetoothConnection.ReadTimeout = 20000;
            Console.WriteLine("Opening BT-connection");
            BluetoothConnection.Open();
            Console.WriteLine("Waiting for NXT to start");

            
            while (Requests.Count() == 0 || !Requests.First().Equals("RDY"))
            {
                QueueRequestFromBuffer(BluetoothConnection);
            }
            



            // Terminate
            Console.WriteLine("Press any-key to terminate");
            Console.ReadKey(); 
        }
    }
}