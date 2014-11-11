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
        const int COLOR_RED_LEFT = 1;
        const int COLOR_RED_RIGHT = 2;
        const int COLOR_BLUE_LEFT = 3;
        const int COLOR_BLUE_RIGHT = 4;
        const int COLOR_BLACK_LEFT = 5;
        const int COLOR_BLACK_RIGHT = 6;
        const int COLOR_GRAY_LEFT = 7;
        const int COLOR_GRAY_RIGHT = 8;
        const int COLOR_WHITE_LEFT = 9;
        const int COLOR_WHITE_RIGHT = 10;

        private static void Main(string[] args)
        {
            PopulateDatabase();
            BluetoothConnection bt = new BluetoothConnection("COM3");
            while (true)
            {
                bt.ConsumePackages();
            }   
        }

        private static void PopulateDatabase()
        {
            // The list of all colors when no database exists
            List<Color> colors = new List<Color> 
            { 
                new Color("RED:LEFT", COLOR_RED_LEFT, 315, 115, 117), 
                new Color("RED:RIGHT", COLOR_RED_RIGHT, 365, 200, 178),
                new Color("BLUE:LEFT", COLOR_BLUE_LEFT, 175, 138, 172), 
                new Color("BLUE:RIGHT", COLOR_BLUE_RIGHT, 207, 219, 233), 
                new Color("BLACK:LEFT", COLOR_BLACK_LEFT, 173, 128, 125), 
                new Color("BLACK:RIGHT", COLOR_BLACK_RIGHT, 200, 295, 263), 
                new Color("GRAY:LEFT", COLOR_GRAY_LEFT, 323, 255, 258), 
                new Color("GRAY:RIGHT", COLOR_GRAY_RIGHT, 418, 398, 352), 
                new Color("WHITE:LEFT", COLOR_WHITE_LEFT, 427, 458, 351), 
                new Color("WHITE:RIGHT", COLOR_WHITE_RIGHT, 500, 488, 437)
            };
            
            // Check if the database contains all colors
            foreach (Color c in colors)
            {
                Color color = Database.Instance.Data.Colors.FirstOrDefault(i => i.Value.Identifier == c.Identifier).Value;
                if (color == null)
                {
                    Database.Instance.Data.AddColor(c);
                }
            }
        }
    }
}
