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
        static void Main(string[] args)
        {
            BluetoothConnection bt = new BluetoothConnection("COM3");

            Console.ReadKey();
            byte[] a = bt.ReadPackageBT();

            foreach (byte b in a)
            {
                Console.Write((char)b);
                Console.Write(" ");
                if ((int)b == 3)
                {
                    break;
                }
            }

            Console.ReadKey();
        }
    }
}