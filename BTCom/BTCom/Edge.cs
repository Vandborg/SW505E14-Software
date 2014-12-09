using BTCom.Interfaces;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BTCom
{
    public class Edge : IEquatable<Edge>, IDecayable
    {
        public double Distance { get; set; }

        public double Blocked { get; set; }
        public int Visited { get; set; }

        public double Weight
        {
            get
            {
                double blocked_probability = Blocked / (Visited == 0 ? 1 : Visited);
                
                if (Math.Abs(blocked_probability - 1) < 0.1)
                {
                    return Double.MaxValue; 
                }

                return Distance / (1 - blocked_probability);
            }
            set { ; }
        }

        public Edge(double distance)
        {
            Distance = distance;
        }

        public bool Equals(Edge other)
        {
            if (ReferenceEquals(null, other)) return false;   // Compare reference to null
            if (ReferenceEquals(this, other)) return true;    // Compare reference to each other

            // Compare properties of both objects
            var sameDistance = Math.Abs(this.Distance - other.Distance) < 1;
            var sameBlocked = Math.Abs(this.Blocked - other.Blocked) < 1;
            var sameVisited = this.Visited == other.Visited;
            var sameWeight = Math.Abs(this.Weight - other.Weight) < 1;

            // Check if all properties are the same
            return sameDistance && sameBlocked && sameVisited && sameWeight;
        }

        public void Decay(double decayRate)
        {
            Blocked *= decayRate;
        }

        public override string ToString()
        {
            return "Distance: '" + Distance + "'. \tBlocked: '" + Blocked + "'. \tVisited: '" + Visited + "'. \tWeight: '" + Weight + "'";
        }
    }
}
