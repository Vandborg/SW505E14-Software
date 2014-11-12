using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BTCom
{
    public class Node : IEquatable<Node>
    {
        public string Name { get; set; }

        private List<KeyValuePair<Node, Edge>> _neighbours = new List<KeyValuePair<Node, Edge>>(4) {
                                                                                                        new KeyValuePair<Node, Edge>(), 
                                                                                                        new KeyValuePair<Node, Edge>(), 
                                                                                                        new KeyValuePair<Node, Edge>(), 
                                                                                                        new KeyValuePair<Node, Edge>()
                                                                                                   };
        public List<KeyValuePair<Node, Edge>> Neighbours
        {
            get { return _neighbours;  }
            set { _neighbours = value; }
        }

        private List<KeyValuePair<Node, Edge>> _blockedNeighbours = new List<KeyValuePair<Node, Edge>>(4) {
                                                                                                              new KeyValuePair<Node, Edge>(), 
                                                                                                              new KeyValuePair<Node, Edge>(), 
                                                                                                              new KeyValuePair<Node, Edge>(), 
                                                                                                              new KeyValuePair<Node, Edge>()
                                                                                                          };
        public List<KeyValuePair<Node, Edge>> BlockedNeighbours
        {
            get { return _blockedNeighbours; }
            set { _blockedNeighbours = value; }
        }

        public Node(string name)
        {
            Name = name;
        }

        public bool Equals(Node other)
        {
            if (ReferenceEquals(null, other)) return false;   // Compare reference to null
            if (ReferenceEquals(this, other)) return true;    // Compare reference to each other

            // Compare properties of both objects
            var sameName = this.Name == other.Name;
            var sameNeighboursAmount = this.Neighbours.Count == other.Neighbours.Count;
            var sameBlockedNeighboursAmount = this.BlockedNeighbours.Count == other.BlockedNeighbours.Count;

            // Check if all properties are the same
            if (!sameName || !sameNeighboursAmount || !sameBlockedNeighboursAmount)
            {
                return false;
            }

            // Check if all neighbours is in the other aswell
            if (this.Neighbours.Any(neighbour => !other.Neighbours.Contains(neighbour)))
            {
                return false;
            }

            // Check if all blocked neighbours is in the other aswell
            if (this.BlockedNeighbours.Any(neighbour => !other.BlockedNeighbours.Contains(neighbour)))
            {
                return false;
            }

            return true;
        }

        public override string ToString()
        {
            string result = "";
            string neighbours = "";
            foreach (var node in Neighbours)
            {
                neighbours += node.Key.Name + ", ";
            }
            result = String.Format("Name: {0} - {1}", Name, neighbours);
            return result;
        }
    }
}
