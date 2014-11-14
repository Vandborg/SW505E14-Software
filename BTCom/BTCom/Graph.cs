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

        public bool AddUndirectedEdge(Tuple<Node, int> nodeOne, Tuple<Node, int> nodeTwo, int weight)
        {
            Edge edge = new Edge(weight);
            return AddUndirectedEdge(nodeOne, nodeTwo, edge);
        }

        public bool AddUndirectedEdge(Tuple<Node, int> nodeOne, Tuple<Node, int> nodeTwo, Edge edge)
        {
            // Check if the nodes exists in the graph
            var containsNodeOne = Nodes.Contains(nodeOne.Item1);
            var containsNodeTwo = Nodes.Contains(nodeTwo.Item1);
            if (!containsNodeOne || !containsNodeTwo) return false;

            // Check if the edge already exists
            var nodeOneHasEdge = nodeOne.Item1.Neighbours.FindAll(x => x.Key == nodeTwo.Item1).Count > 0;
            var nodeTwoHasEdge = nodeOne.Item1.Neighbours.FindAll(x => x.Key == nodeOne.Item1).Count > 0;
            if (nodeOneHasEdge || nodeTwoHasEdge) return false;

            nodeOne.Item1.Neighbours[nodeOne.Item2] = new KeyValuePair<Node, Edge>(nodeTwo.Item1, edge);
            nodeTwo.Item1.Neighbours[nodeTwo.Item2] = new KeyValuePair<Node, Edge>(nodeOne.Item1, edge);

            return true;
        }

        public bool RemoveUndirectedEdge(Node nodeOne, Node nodeTwo)
        {
            // Check if the nodes exists in the graph
            var containsNodeOne = Nodes.Contains(nodeOne);
            var containsNodeTwo = Nodes.Contains(nodeTwo);
            if (!containsNodeOne || !containsNodeTwo) return false;

            // Remove edges from both nodes
            var keyvalue = nodeOne.Neighbours.Single(x => x.Key == nodeTwo);
            int index = nodeOne.Neighbours.IndexOf(keyvalue);

            nodeOne.Neighbours[index] = new KeyValuePair<Node,Edge>();

            keyvalue = nodeTwo.Neighbours.Single(x => x.Key == nodeOne);
            index = nodeTwo.Neighbours.IndexOf(keyvalue);

            nodeTwo.Neighbours[index] = new KeyValuePair<Node, Edge>();

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
            foreach (var checkNodePair in node.Neighbours)
            {
                Node checkNode = checkNodePair.Key;

                if (checkNode == null)
                {
                    continue;
                }
                // Remove edges from both nodes
                var keyvalue = checkNode.Neighbours.SingleOrDefault(x => x.Key == node);
                
                if (keyvalue.Key != null)
                {
                    int index = checkNode.Neighbours.IndexOf(keyvalue);

                    checkNode.Neighbours[index] = new KeyValuePair<Node, Edge>();
                    checkNode.BlockedNeighbours[index] = new KeyValuePair<Node, Edge>();
                }
                
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

        public Path ShortestPath(String from, String to, String ignore = null)
        {
            Node a = getNode(from);
            Node b = getNode(to);

            Node ignoreNode = null;

            if (ignore != null)
            {
                ignoreNode = getNode(ignore);
            }

            return ShortestPath(a, b, ignoreNode);
        }

        public Path ShortestPath(Node from, Node to, Node ignore = null)
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
                if (node == null)
                {
                    continue;
                }

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
                    if (node == null)
                    {
                        continue;
                    }

                    int newDistance = distance.Find(x => x.Key.Equals(node)).Value;

                    if (newDistance < oldDistance)
                    {
                        selectedNode = node;
                        oldDistance = newDistance;
                    }
                }

                // Remove the node from u
                Q.Remove(selectedNode);

                foreach (KeyValuePair<Node, Edge> neighbour in selectedNode.Neighbours)
                {
                    if (neighbour.Key == null)
                    {
                        continue;
                    }

                    // If selected the from node, ignore the ignore node
                    if (ignore != null && selectedNode.Equals(from) && neighbour.Key.Equals(ignore))
                    {
                        continue;
                    }
                    
                    // Find the distance to the selected
                    int distanceToSelectedNode = distance.Find(x => x.Key.Equals(selectedNode)).Value;

                    // Find the distance to the selected neighbour
                    int distanceToNeighbour = selectedNode.Neighbours.Find(x => x.Key != null && x.Key.Equals(neighbour.Key)).Value.Weight;

                    // Add the two distances
                    int alt = distanceToSelectedNode + distanceToNeighbour;

                    KeyValuePair<Node, int> neighbourDistance = distance.Find(x => x.Key.Equals(neighbour.Key));
                    KeyValuePair<Node, Node> neighbourPrevious = previous.Find(x => x.Key.Equals(neighbour.Key));

                    // Check if this new distance (alt) is shorter than what we know
                    if(alt < neighbourDistance.Value)
                    {
                        // Update the distance
                        distance.RemoveAll(x => x.Key.Equals(neighbour.Key));
                        distance.Add(new KeyValuePair<Node, int>(neighbour.Key, alt));

                        // Set previous of neighbour to selectednode
                        previous.RemoveAll(x => x.Key.Equals(neighbour.Key));
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
                KeyValuePair<Node, Node> newPreviousNode = previous.Find(x => x.Key != null && x.Key.Equals(lastNode));

                if (lastNode.Equals(from))
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
