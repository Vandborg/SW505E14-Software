using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using BTCom.Exceptions;

namespace BTCom
{
    public class ForkliftPath
    {
        private static readonly char[,] navigationTable = new char[4, 4] { {'#','L','S','R'}, {'R','#','L','S'},
                                                                           {'S','R','#','L'}, {'L','S','R','#'} };

        private readonly List<Char> forkliftPath = new List<Char>();

        private Path path = null;
        private Node initialNode = null;

        public ForkliftPath(Path path, Node initialNode)
        {
            // Check if the initial node is also the first node in the path
            if (initialNode.Equals(path.Nodes.FirstOrDefault()))
            {
                throw new NodeException(initialNode, "First- and ignore node cannot be the same");
            }

            // Check if the initial node is a neighbour of the first node in the path
            if (path.Nodes.FirstOrDefault().Neighbours.FindAll(x => x.Key != null && x.Key.Equals(initialNode)).Count == 0)
            {
                throw new NodeException(initialNode, "Could not find initial node '" + initialNode.Name + "' in the neighbours of node '" + path.Nodes.FirstOrDefault().Name + "'");
            }

            this.path = path;
            this.initialNode = initialNode;

            if (path.Nodes.Count <= 1)
            {
                return;
            }

            int initialIndex = path.Nodes[0].Neighbours.FindIndex(x => x.Key != null && x.Key.Equals(initialNode));

            int fromIndex = initialIndex;
            int toIndex = -1;

            for (int i = 0; i < path.Nodes.Count - 1; i++)
            {
                Node fromNode = null;
                Node thisNode = path.Nodes[i];
                Node toNode = path.Nodes[i+1];

                if (i != 0)
                {
                    fromNode = path.Nodes[i - 1];
                    fromIndex = thisNode.Neighbours.FindIndex(x => x.Key != null && x.Key.Equals(fromNode));
                }

                toIndex = thisNode.Neighbours.FindIndex(x => x.Key != null && x.Key.Equals(toNode));

                forkliftPath.Add(navigationTable[fromIndex, toIndex]);
            }
        }

        // Method to convert a ForkliftPath to the desired NXT format
        public string getDirections()
        {
            forkliftPath.Reverse();
            String s = new string(forkliftPath.ToArray());
            forkliftPath.Reverse();

            return s;
        }

        public override string ToString()
        {
            String s = "";

            s += "Initial node:\t'" + initialNode.Name + "'\n";

            int i = 0;
            for (i = 0; i < path.Nodes.Count - 1; i++)
            {
                Node node = path.Nodes[i];

                s += "Action in node:\t'" + node.Name + "':\t" + forkliftPath[i] + "\n";
            }

            s += "Arrive in node:\t'" + path.Nodes[i].Name + "'.";

            return s;
        }
    }
}
