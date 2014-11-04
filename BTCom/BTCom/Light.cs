using System;
using System.Collections.Generic;
using System.Dynamic;
using System.Linq;
using System.Linq.Expressions;
using System.Text;
using System.Threading.Tasks;

namespace BTCom
{
    public class Light : DataEntry
    {
        public string Name { get; set; }
        public int LightLevel { get; set; }

        public Light() { }

        public Light(string name, int id)
        {
            Name = name;
            Identifier = id;
        }

        public Light(string name, int id, int lightLevel) : this(name, id)
        {
            LightLevel = lightLevel;
        }

        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;   // Compare reference to null
            if (obj.GetType() != GetType()) return false;   // Compare types
            if (ReferenceEquals(this, obj)) return true;    // Compare reference to each other

            var other = (Light) obj;

            // Compare properties of both objects
            var sameIdentifier = this.Identifier == other.Identifier;
            var sameName = this.Name == other.Name;
            var sameLightLevel = this.LightLevel == other.LightLevel;

            // Check if all properties are the same
            return sameIdentifier && sameName && sameLightLevel;
        }

        public override string ToString()
        {
            return base.ToString() + " - " + "Name: '" + Name + "'. Light Level: '" + LightLevel + "'.";
        }
    }
}
