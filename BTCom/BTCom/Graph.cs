using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BTCom
{
    public class Graph : DataEntry
    {
        private List<Node> _nodes = new List<Node>();
        public List<Node> Nodes
        {
            get { return _nodes;  }
            set { _nodes = value; }
        }

        public Graph()
        {
            
        }

        public Graph(int id)
        {
            Identifier = id;
        }

        public bool AddUndirectedEdge(Node nodeOne, Node nodeTwo, Edge edge)
        {
            // Check if the nodes exists in the graph
            var containsNodeOne = Nodes.Contains(nodeOne);
            var containsNodeTwo = Nodes.Contains(nodeTwo);
            if (!containsNodeOne || !containsNodeTwo) return false;

            // Check if the edge already exists
            var nodeOneHasEdge = nodeOne.Neighbours.FindAll(x => x.Key == nodeTwo).Count > 0;
            var nodeTwoHasEdge = nodeOne.Neighbours.FindAll(x => x.Key == nodeOne).Count > 0;
            if (nodeOneHasEdge || nodeTwoHasEdge) return false;

            nodeOne.Neighbours.Add(new KeyValuePair<Node, Edge>(nodeTwo, edge));
            nodeTwo.Neighbours.Add(new KeyValuePair<Node, Edge>(nodeOne, edge));

            return true;
        }

        public bool RemoveUndirectedEdge(Node nodeOne, Node nodeTwo)
        {
            // Check if the nodes exists in the graph
            var containsNodeOne = Nodes.Contains(nodeOne);
            var containsNodeTwo = Nodes.Contains(nodeTwo);
            if (!containsNodeOne || !containsNodeTwo) return false;

            // Remove edges from both nodes
            nodeOne.Neighbours.RemoveAll(x => x.Key == nodeTwo);
            nodeTwo.Neighbours.RemoveAll(x => x.Key == nodeOne);

            return true;
        }

        public bool AddNode(Node node)
        {
            // Check if the node is already in the list
            var containsNode = Nodes.Contains(node);
            if (containsNode) return false;

            // Add the node
            Nodes.Add(node);

            return true;
        }

        public bool RemoveNode(Node node)
        {
            // Check if the node is already in the list
            var containsNode = Nodes.Contains(node);
            if (!containsNode) return false;

            // Remove all egdes to the node removed
            foreach (Node checkNode in Nodes)
            {
                checkNode.Neighbours.RemoveAll(x => x.Key == node);
                checkNode.BlockedNeighbours.RemoveAll(x => x.Key == node);
            }

            // Remove the node
            Nodes.Remove(node);

            return true;
        }

        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;   // Compare reference to null
            if (obj.GetType() != GetType()) return false;   // Compare types
            if (ReferenceEquals(this, obj)) return true;    // Compare reference to each other

            var other = (Graph) obj;

            // Compare properties of both objects
            var sameIdentifier = this.Identifier == other.Identifier;
            var sameNodeLength = this.Nodes.Count == other.Nodes.Count;

            // Check if all properties are the same
            return sameIdentifier && sameNodeLength;
        }
    }
}
