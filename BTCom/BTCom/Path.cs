using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BTCom
{
    public class Path : DataEntry
    {
        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;   // Compare reference to null
            if (obj.GetType() != GetType()) return false;   // Compare types
            if (ReferenceEquals(this, obj)) return true;    // Compare reference to each other

            var other = (Path) obj;

            // Compare properties of both objects
            var sameIdentifier = this.Identifier == other.Identifier;

            // Check if all properties are the same
            return sameIdentifier;
        }
    }
}
