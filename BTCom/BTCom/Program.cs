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
        private static void Main(string[] args)
        {
            PopulateDatabase();
        }

        private static void PopulateDatabase()
        {
            // The list of all colors when no database exists
            List<Color> colors = new List<Color> 
            { 
                new Color("RED:L", 1, 315, 115, 117), 
                new Color("RED:R", 2, 365, 200, 178),
                new Color("BLUE:L", 3, 175, 138, 172), 
                new Color("BLUE:R", 4, 207, 219, 233), 
                new Color("BLACK:L", 5, 173, 128, 125), 
                new Color("BLACK:R", 6, 200, 295, 263), 
                new Color("GRAY:L", 7, 323, 255, 258), 
                new Color("GRAY:R", 8, 418, 398, 352), 
                new Color("WHITE:L", 9, 427, 458, 351), 
                new Color("WHITE:R", 10, 500, 488, 437)
            };
            
            // Check if the database contains all colors
            foreach (Color c in colors)
            {
                Color color = Database.Instance.Data.Colors.FirstOrDefault(i => i.Value.Name == c.Name).Value;
                if (color == null)
                {
                    Database.Instance.Data.AddColor(c);
                }
            }
        }
    }
}