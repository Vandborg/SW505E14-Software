using System;
using System.Collections.Generic;
using System.Dynamic;
using System.Linq;
using System.Linq.Expressions;
using System.Text;
using System.Threading.Tasks;

namespace BTCom
{
    public class Color : DataEntry
    {
        public string Name { get; set; }
        public int Red { get; set; }
        public int Green { get; set; }
        public int Blue { get; set; }

        public Color(){}
        
        public Color(string name, int id)
        {
            Name = name;
            Identifier = id;
        }

        public Color(string name, int id, int red, int green, int blue) : this(name, id)
        {
            Rgb(red, green, blue);
        }

        public void Rgb(int red, int green, int blue)
        {
            Red = red;
            Green = green;
            Blue = blue;
        }

        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;   // Compare reference to null
            if (obj.GetType() != GetType()) return false;   // Compare types
            if (ReferenceEquals(this, obj)) return true;    // Compare reference to each other

            var other = (Color) obj;

            // Compare properties of both objects
            var sameIdentifier = this.Identifier == other.Identifier;
            var sameName = this.Name == other.Name;
            var sameRed = this.Red == other.Red;
            var sameGreen = this.Green == other.Green;
            var sameBlue = this.Blue == other.Blue;

            // Check if all properties are the same
            return sameIdentifier && sameName && sameRed && sameGreen && sameBlue;
        }

        public override string ToString()
        {
            return base.ToString() + " - " + "Name: '" + Name + "'. Red: '" + Red + "'. Green: '" + Green + "'. Blue: '" + Blue + "'.";
        }
    }
}
