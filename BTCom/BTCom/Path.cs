using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BTCom
{
    public class Path : DataEntry
    {
        private List<Node> _path = new List<Node>();

        private static char[,] navigationTable = new char[4, 4] { {'#','L','S','R'}, {'R','#','L','S'},
                                                                      {'S','R','#','L'}, {'L','S','R','#'} };
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

        public string convertToCarPath(Node initialNode)
        {
            int initialIndex = Nodes[0].Neighbours.FindIndex(x => x.Key != null && x.Key.Equals(initialNode));
            
            List <Char> carPath = new List<Char>();

            int fromIndex = initialIndex;
            int toIndex = -1;

            for (int i = 0; i < Nodes.Count - 1; i++)
            {
                Node fromNode = Nodes[i];
                Node toNode = Nodes[i+1];

                // Find the index of the edge from fromNode to toNode
                toIndex = fromNode.Neighbours.FindIndex(x => x.Key != null && x.Key.Equals(toNode));

                carPath.Add(navigationTable[fromIndex, toIndex]);

                fromIndex = toNode.Neighbours.FindIndex(x => x.Key != null && x.Key.Equals(fromNode));
            }

            // carPath.Reverse();

            return new String(carPath.ToArray());
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
