using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;
using SQLite;
 
namespace BTCom
{
    class Program
    {

        public static char[] ReadCharsOnBuffer(SerialPort BluetoothConnection)
        {
            char[] output = new char[BluetoothConnection.ReadBufferSize];
           
            for (int i = 0; BluetoothConnection.BytesToRead > 0; i++)
            {
                output[i] = (char)BluetoothConnection.ReadChar();
            }

            return output; 
        }

        static void Main(string[] args)
        {
            Console.WriteLine("Connecting to DataBase");
            var db = new SQLiteConnection("DataBase");
            Console.WriteLine("Connected to DataBase");
            db.CreateTable<Setting>();

            Setting ReadSetting = db.Table<Setting>().FirstOrDefault();
            if (true)
            {
                Console.WriteLine("Read: " + ReadSetting.BlackRed + " from DataBase");
                Console.ReadKey();
            }


            
            SerialPort BluetoothConnection = new SerialPort("COM3");
            BluetoothConnection.ReadTimeout = 20000;
            Console.WriteLine("Opening BT-connection");
            BluetoothConnection.Open();
            Console.WriteLine("Waiting for NXT to start");

            while (ReadCharsOnBuffer(BluetoothConnection)[0] != 'r')
            {
                
            }
            BluetoothConnection.DiscardInBuffer();
            Console.WriteLine("NXT has Started");

            Console.WriteLine("Press any key after you have pressed send on device");
            Console.ReadKey();


            char[] RNFXTS = ReadCharsOnBuffer(BluetoothConnection);
            string SAGER = new String(RNFXTS);
            int ReadNumberFromNXT = int.Parse(SAGER);

            Console.WriteLine("Data Recieved: " + ReadNumberFromNXT);

            Setting TestSetting = new Setting();
            TestSetting.BlackRed = ReadNumberFromNXT;
            Console.WriteLine("Inserting: " + ReadNumberFromNXT + " into DataBase");
            db.Insert(TestSetting);            
            
            Console.WriteLine("Press any-key to terminate");
            Console.ReadKey(); 
        }
    }
}