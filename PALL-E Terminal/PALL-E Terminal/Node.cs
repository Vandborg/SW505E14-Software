using System;
using System.Collections.Generic;
using System.Linq;

namespace PALL_E_Terminal
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

        public bool HasPallet {
            get { return Database.Instance.Data.Pallets.Any(palletPair => palletPair.Value.Location != null && palletPair.Value.Location.Equals(this)); }
            set {} 
        }

        public Pallet Pallet
        {
            get { return Database.Instance.Data.Pallets.FirstOrDefault(palletPair => palletPair.Value.Location != null && palletPair.Value.Location.Equals(this)).Value; }
            set { }
        }

        public bool IsPalletNode { get; set; }

        public Node(string name, bool isPalletNode = false)
        {
            Name = name;
            IsPalletNode = isPalletNode;
        }

        public bool Equals(Node other)
        {
            if (ReferenceEquals(null, other)) return false;   // Compare reference to null
            if (ReferenceEquals(this, other)) return true;    // Compare reference to each other

            // Compare properties of both objects
            var sameName = this.Name == other.Name;
            var sameNeighboursAmount = this.Neighbours.Count == other.Neighbours.Count;

            // Check if all properties are the same
            if (!sameName || !sameNeighboursAmount)
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
                if (node.Key == null) continue;
                neighbours += Neighbours.IndexOf(node) + ": " + node.Key.Name + ", ";
            }
            result = String.Format("Name: {0} - {1}", Name, neighbours);
            return result;
        }
    }
}
