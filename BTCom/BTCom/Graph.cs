using System;
using System.Collections.Generic;
using System.IO.Packaging;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using BTCom.Exceptions;
using BTCom.Interfaces;

namespace BTCom
{
    public class Graph : DataEntry, IDecayable
    {
        private List<Node> _nodes = new List<Node>();
        public List<Node> Nodes
        {
            get { return _nodes;  }
            set { _nodes = value; }
        }

        private List<Tuple<Node, Node>> _blockedEdges = new List<Tuple<Node, Node>>();
        public List<Tuple<Node, Node>> BlockedEdges
        {
            get { return _blockedEdges; }
            set { _blockedEdges = value; }
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
            foreach (KeyValuePair<Node, Edge> checkNodePair in node.Neighbours)
            {
                Node checkNode = checkNodePair.Key;

                if (checkNode == null)
                {
                    continue;
                }

                // Remove edges from both nodes
                KeyValuePair<Node, Edge> keyvalue = checkNode.Neighbours.Find(x => x.Key.Equals(node));
                
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

        public bool BlockEdge(String nodeOne, String nodeTwo)
        {
            Node a = getNode(nodeOne);
            Node b = getNode(nodeTwo);
            return BlockEdge(a, b);
        }

        public bool BlockEdge(Node nodeOne, Node nodeTwo)
        {
            var edge = new Tuple<Node, Node>(nodeOne, nodeTwo);

            if (BlockedEdges.Contains(edge)) return false;

            BlockedEdges.Add(edge);

            KeyValuePair<Node, Edge> edgeOne = nodeOne.Neighbours.Single(x => x.Key != null && x.Key.Equals(nodeTwo));
            int index = nodeOne.Neighbours.IndexOf(edgeOne);
            nodeOne.BlockedNeighbours[index] = edgeOne;
            nodeOne.Neighbours[index] = new KeyValuePair<Node, Edge>();

            KeyValuePair<Node, Edge> edgeTwo = nodeTwo.Neighbours.Single(x => x.Key != null && x.Key.Equals(nodeOne));
            index = nodeTwo.Neighbours.IndexOf(edgeTwo);
            nodeTwo.BlockedNeighbours[index] = edgeTwo;
            nodeTwo.Neighbours[index] = new KeyValuePair<Node, Edge>();

            if (BreathFirstSearch().Count != Nodes.Count)
            {
                throw new Exception("Graph is now not connected");
            }
            
            return true;
        }

        public bool UnblockEdge(String nodeOne, String nodeTwo)
        {
            Node a = getNode(nodeOne);
            Node b = getNode(nodeTwo);
            return UnblockEdge(a, b);
        }

        public bool UnblockEdge(Node nodeOne, Node nodeTwo)
        {
            var edge = new Tuple<Node, Node>(nodeOne, nodeTwo);

            if (!BlockedEdges.Contains(edge)) return false;

            BlockedEdges.Remove(edge);
            var edgeOne = nodeOne.BlockedNeighbours.Single(x => x.Key != null && x.Key.Equals(nodeTwo));
            nodeOne.Neighbours.Add(edgeOne);
            nodeOne.BlockedNeighbours.Remove(edgeOne);
            

            var edgeTwo = nodeTwo.BlockedNeighbours.Single(x => x.Key != null && x.Key.Equals(nodeOne));
            nodeTwo.Neighbours.Add(edgeTwo);
            nodeTwo.BlockedNeighbours.Remove(edgeTwo);

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

        public void Decay(double decayRate)
        {
            List<Edge> edges = new List<Edge>();

            // Find all unique edges and add them to a list
            foreach (Node node in Nodes)
            {
                foreach (KeyValuePair<Node, Edge> edge in node.Neighbours)
                {
                    if (edge.Value != null && !edges.Contains(edge.Value))
                    {
                        edges.Add(edge.Value);    
                    }
                }

                foreach (KeyValuePair<Node, Edge> edge in node.BlockedNeighbours)
                {
                    if (edge.Value != null && !edges.Contains(edge.Value))
                    {
                        edges.Add(edge.Value);
                    }
                }
            }

            // Go through the edges and decay them
            foreach (Edge edge in edges)
            {
                edge.Decay(decayRate);
            }
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
            if (from == null)
            {
                throw new NodeException("From-node cannot be null");
            }

            if (to == null)
            {
                throw new NodeException("To-node cannot be null");
            }

            // Make sure that the nodes are in the graph
            if (!(Nodes.FindAll(x => x.Equals(from)).Count > 0 && Nodes.FindAll(x => x.Equals(to)).Count > 0))
            {
                throw new Exception("Either 'from'- or 'to'-node not in graph!");
            }

            List<Node> Q = new List<Node>();

            List<KeyValuePair<Node, double>> distance = new List<KeyValuePair<Node, double>>();
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
                    distance.Add(new KeyValuePair<Node, double>(node, Double.MaxValue));

                    // Set the previous node for all nodes to null (undefined)
                    previous.Add(new KeyValuePair<Node, Node>(node, null));
                }

                // Add the node to Q
                Q.Add(node);
            }

            // The distance to the node "from" is always 0
            distance.Add(new KeyValuePair<Node, double>(from, 0));

            // While there are nodes to inspect
            while (Q.Count > 0)
            {
                // Find the node with lowest distance in Q
                Node selectedNode = Q.First();
                double oldDistance = Int32.MaxValue;

                foreach (Node node in Q)
                {
                    if (node == null)
                    {
                        continue;
                    }

                    double newDistance = distance.Find(x => x.Key.Equals(node)).Value;

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
                    double distanceToSelectedNode = distance.Find(x => x.Key.Equals(selectedNode)).Value;

                    // Find the distance to the selected neighbour
                    double distanceToNeighbour = selectedNode.Neighbours.Find(x => x.Key != null && x.Key.Equals(neighbour.Key)).Value.Weight;

                    // Add the two distances
                    double alt = distanceToSelectedNode + distanceToNeighbour;

                    double neighbourDistance = distance.Find(x => x.Key.Equals(neighbour.Key)).Value;

                    // Check if this new distance (alt) is shorter than what we know
                    if(alt < neighbourDistance)
                    {
                        // Update the distance
                        distance.RemoveAll(x => x.Key.Equals(neighbour.Key));
                        distance.Add(new KeyValuePair<Node, double>(neighbour.Key, alt));

                        // Set previous of neighbour to selectednode
                        previous.RemoveAll(x => x.Key.Equals(neighbour.Key));
                        previous.Add(new KeyValuePair<Node, Node>(neighbour.Key, selectedNode));
                    }
                }
            }

            // Create a new path
            Path p = new Path();

            // Start the path from the target (Path will later be inverted)
            Node lastNode = to;
            bool previousNode = true;

            // Add the last node to path
            p.Nodes.Add(lastNode);

            // Continue the loop as long as there are a previous node
            while (previousNode)
            {
                // Find the previous node
                KeyValuePair<Node, Node> newPreviousNode = previous.Find(x => x.Key != null && x.Key.Equals(lastNode));

                // Check if the last node is the start
                // There is a path, everything went well
                if (lastNode.Equals(from))
                {
                    previousNode = false;
                }
                // The last node has a previous node, everything is good
                // However, the path is still not finished, so continue the loop
                else if (newPreviousNode.Value != null)
                {
                    p.Nodes.Add(newPreviousNode.Value);
                    lastNode = newPreviousNode.Value;
                }
                // We are not at the start node, and not at a valid node,
                // hence, there must be a gab in the path. Error!
                else
                {
                    return null;
                }
            }

            // Reserve the graph
            p.Nodes.Reverse();

            return p;
        }

        public List<Node> BreathFirstSearch()
        {
            Queue<Node> Q = new Queue<Node>();
            List<Node> V = new List<Node>();

            V.Add(Nodes.FirstOrDefault());
            Q.Enqueue(V.FirstOrDefault());

            while (Q.Count != 0)
            {
                Node t = Q.Dequeue();
                foreach (var edges in t.Neighbours)
                {
                    if (edges.Key != null)
                    {
                        Node u = edges.Key;
                        if (!V.Contains(u))
                        {
                            V.Add(u);
                            Q.Enqueue(u);
                        }
                    }
                }
            }

            return V;
        } 

        public Path PathToBlockedEdge(Node startNode, Tuple<Node,Node> edge)
        {   
            var PathOne = ShortestPath(startNode, edge.Item1);
            var PathTwo = ShortestPath(startNode, edge.Item2);

            if (PathOne.Weight >= PathTwo.Weight)
            {
                return PathTwo;
            }
            else
            {
                return PathOne;
            }
        }
        
    }
}
