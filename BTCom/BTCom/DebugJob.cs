using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace BTCom
{
    public class DebugJob : DataEntry
    {
        public byte Type { get; set; }
        public string Directions { get; set; }

        public DebugJob()
        {
            
        }

        public DebugJob(byte type, String directions)
        {
            Regex rgx = new Regex(@"([L]*[R]*[S]*)*");
            if (rgx.IsMatch(directions))
            {
                Type = type;
                Directions = directions;
            }
            else
            {
                throw new FormatException("Can only contain L R or S characters");
            }
        }

        public DebugJob(int identifier, byte type, String directions)
            : this(type, directions)
        {
            Identifier = identifier;
        }

        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;   // Compare reference to null
            if (obj.GetType() != GetType()) return false;   // Compare types
            if (ReferenceEquals(this, obj)) return true;    // Compare reference to each other

            var other = (DebugJob) obj;

            // Compare properties of both objects
            var sameIdentifier = this.Identifier == other.Identifier;
            var sameType = this.Type.Equals(other.Type);
            var sameDirections = this.Directions.Equals(other.Directions);

            // Check if all properties are the same
            return sameIdentifier && sameType && sameDirections;
        }

        public byte[] GetByes()
        {
            // Reverse the string
            string s = new string(this.Directions.ToCharArray().Reverse().ToArray());

            // Convert the string to byte array
            return Encoding.ASCII.GetBytes(s);
        }

        public override string ToString()
        {
            return "D: '" + this.Directions+ "'";
        }
    }
}
