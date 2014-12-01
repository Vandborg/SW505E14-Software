using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace BTCom
{
    public class DebugJob : Job
    {
        public string Directions { get; set; }

        public DebugJob(int identifier, String directions)
        {
            Identifier = identifier;

            string dir = directions.ToUpper();

            Regex lrsRegex = new Regex("^[LRS]*$");

            if (!lrsRegex.IsMatch(dir))
            {
                throw new FormatException("Directions can only be a combination of 'L', 'R' and 'S'");
            }

            Directions = dir;
        }

        public override Path GetPath()
        {
            // Debug jobs does not consider an actual path, so nothing should be used (aka updated)
            return new Path();
        }

        public override byte[] GetBytes()
        {
            return Encoding.ASCII.GetBytes(Directions);
        }

        public override byte GetJobTypeBytes()
        {
            return BluetoothConnection.TYPE_NAVIGATE_TO;
        }

        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;   // Compare reference to null
            if (obj.GetType() != GetType()) return false;   // Compare types
            if (ReferenceEquals(this, obj)) return true;    // Compare reference to each other

            var other = (NavigateJob)obj;

            // Compare properties of both objects
            var sameIdentifier = this.Identifier == other.Identifier;
            var sameDirectionns = this.Directions.Equals(other.Directions);

            // Check if all properties are the same
            return sameIdentifier && sameDirectionns;
        }

        public override string ToString()
        {
            return "#" + ID() + ". Type: 'debug'" + ". Directions: '" + Directions.ToUpper() + "'";
        }
    }
}
