using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BTCom
{
    public class Edge : IEquatable<Edge>
    {
        public int Weight { get; set; }

        public Edge(int weight)
        {
            Weight = weight;
        }

        public bool Equals(Edge other)
        {
            if (ReferenceEquals(null, other)) return false;   // Compare reference to null
            if (ReferenceEquals(this, other)) return true;    // Compare reference to each other

            // Compare properties of both objects
            var sameWeight = this.Weight == other.Weight;

            // Check if all properties are the same
            return sameWeight;
        }
    }
}
