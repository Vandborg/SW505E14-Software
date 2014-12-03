using System;

namespace BTCom
{
    public class Pallet : DataEntry
    {
        public String Name { get; set; }

        public Node Location { get; set; }

        public Pallet()
        {
            
        }

        public Pallet(int identifier, String name, Node location)
        {
            Identifier = identifier;
            Name = name;
            Location = location;
        }

        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;   // Compare reference to null
            if (obj.GetType() != GetType()) return false;   // Compare types
            if (ReferenceEquals(this, obj)) return true;    // Compare reference to each other

            var other = (Pallet) obj;

            // Compare properties of both objects
            var sameIdentifier = this.Identifier == other.Identifier;

            // Check if all properties are the same
            return sameIdentifier;
        }

        public override string ToString()
        {
            return "Name '" + Name + "', Location: '" + Location.Name + "'";
        }
    }
}
