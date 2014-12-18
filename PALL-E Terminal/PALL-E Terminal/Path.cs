using System.Collections.Generic;
using System.Linq;

namespace PALL_E_Terminal
{
    public class Path : DataEntry
    {
        private List<Node> _path = new List<Node>();

        public double Weight 
        { 
            get
            {
                double weight = 0;
                for(int i = 0; i < Nodes.Count-1; i++)
                {
                    int index = Nodes[i].Neighbours.FindIndex(x => x.Key != null && x.Key.Equals(Nodes[i + 1]));
                    weight += Nodes[i].Neighbours[index].Value.Weight;
                }
                return weight;
            }
        }

        public List<Node> Nodes
        {
            get { return _path; }
            set { _path = value; }
        } 

        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;   // Compare reference to null
            if (obj.GetType() != GetType()) return false;   // Compare types
            if (ReferenceEquals(this, obj)) return true;    // Compare reference to each other

            var other = (Path) obj;

            // Compare properties of both objects
            var sameIdentifier = this.Identifier == other.Identifier;
            var samePathLength = this.Nodes.Count == other.Nodes.Count;
            var samePathNodes = Nodes.All(node => node.Equals(other.Nodes[this.Nodes.IndexOf(node)]));

            // Check if all properties are the same
            return sameIdentifier && samePathLength && samePathNodes;
        }

        public override string ToString()
        {
            string s = "";

            foreach (Node node in Nodes)
            {
                s += " -> " + node.Name;
            }

            return s;
        }
    }
}
