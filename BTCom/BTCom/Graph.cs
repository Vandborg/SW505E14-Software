using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using BTCom.Exceptions;

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

        public bool AddUndirectedEdge(Node nodeOne, Node nodeTwo, int weight)
        {
            Edge edge = new Edge(weight);
            return AddUndirectedEdge(nodeOne, nodeTwo, edge);
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

        public Node getNode(string name)
        {
            List<Node> nodesWithName = Nodes.FindAll(x => x.Name.ToLower().Equals(name.ToLower()));

            // Check if no nodes were found
            if (nodesWithName.Count == 0)
            {
                throw new NodeException("Node with name '" + name.ToLower() + "' not found.");
            }
            // Check if multiple nodes were found
            else if(nodesWithName.Count > 1)
            {
                throw new NodeException("Multiple nodes with same name: '" + name.ToLower() + "'.");
            }
            // Found exactly one node
            else
            {
                return nodesWithName.FirstOrDefault();
            }
        }

        public Path ShortestPath(String from, String to)
        {
            Node a = getNode(from);
            Node b = getNode(to);

            return ShortestPath(a, b);
        }

        public Path ShortestPath(Node from, Node to)
        {
            // Make sure that the nodes are in the graph
            if (!(Nodes.FindAll(x => x.Equals(from)).Count > 0 && Nodes.FindAll(x => x.Equals(to)).Count > 0))
            {
                throw new Exception("Either 'from'- or 'to'-node not in graph!");
            }

            List<Node> Q = new List<Node>();

            List<KeyValuePair<Node, int>> distance = new List<KeyValuePair<Node, int>>();
            List<KeyValuePair<Node, Node>> previous = new List<KeyValuePair<Node, Node>>();

            foreach (Node node in Nodes)
            {
                if (!node.Equals(from))
                {
                    // Set the distance of all nodes to infinity
                    distance.Add(new KeyValuePair<Node, int>(node, Int32.MaxValue));

                    // Set the previous node for all nodes to null (undefined)
                    previous.Add(new KeyValuePair<Node, Node>(node, null));
                }

                // Add the node to Q
                Q.Add(node);
            }

            // The distance to the node "from" is always 0
            distance.Add(new KeyValuePair<Node, int>(from, 0));

            // While there are nodes to inspect
            while (Q.Count > 0)
            {
                // Find the node with lowest distance in Q
                Node selectedNode = Q.First();
                int oldDistance = Int32.MaxValue;

                foreach (Node node in Q)
                {
                    int newDistance = distance.Find(x => x.Key.Equals(node)).Value;

                    if (selectedNode == null || newDistance < oldDistance)
                    {
                        selectedNode = node;
                        oldDistance = newDistance;
                    }
                }

                // Remove the node from u
                Q.Remove(selectedNode);

                foreach (KeyValuePair<Node, Edge> neighbour in selectedNode.Neighbours)
                {
                    // Find the distance to the selected
                    int distanceToSelectedNode = distance.Find(x => x.Key.Equals(selectedNode)).Value;

                    // Find the distance to the selected neighbour
                    int distanceToNeighbour = selectedNode.Neighbours.Find(x => x.Key.Equals(neighbour.Key)).Value.Weight;

                    // Add the two distances
                    int alt = distanceToSelectedNode + distanceToNeighbour;

                    KeyValuePair<Node, int> neighbourDistance = distance.Find(x => x.Key.Equals(neighbour.Key));
                    KeyValuePair<Node, Node> neighbourPrevious = previous.Find(x => x.Key.Equals(neighbour.Key));

                    // Check if this new distance (alt) is shorter than what we know
                    if(alt < neighbourDistance.Value)
                    {
                        // Update the distance
                        distance.Remove(neighbourDistance);
                        distance.Add(new KeyValuePair<Node, int>(neighbour.Key, alt));

                        // Set previous of neighbour to selectednode
                        previous.Remove(neighbourPrevious);
                        previous.Add(new KeyValuePair<Node, Node>(neighbour.Key, selectedNode));
                    }
                }
            }

            Path p = new Path();

            Node lastNode = to;
            bool previousNode = true;

            p.Nodes.Add(lastNode);

            while (previousNode)
            {
                KeyValuePair<Node, Node> newPreviousNode = previous.Find(x => x.Key.Equals(lastNode));

                if (newPreviousNode.Key == null && newPreviousNode.Value == null)
                {
                    // There is a path, everything went well
                    previousNode = false;
                }
                else if (newPreviousNode.Value != null)
                {
                    p.Nodes.Add(newPreviousNode.Value);
                    lastNode = newPreviousNode.Value;
                }
                else
                {
                    // Gab in path, unable to create path!
                    return null;
                }
            }

            p.Nodes.Reverse();

            return p;
        }
    }
}
