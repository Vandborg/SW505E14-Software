using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BTCom
{
    public class ForkliftPath
    {
        private static readonly char[,] navigationTable = new char[4, 4] { {'#','L','S','R'}, {'R','#','L','S'},
                                                                           {'S','R','#','L'}, {'L','S','R','#'} };

        private readonly List<Char> forkliftPath = new List<Char>();

        private Path path = new Path();

        public ForkliftPath(Path path, Node initialNode)
        {
            this.path = path;

            int initialIndex = path.Nodes[0].Neighbours.FindIndex(x => x.Key != null && x.Key.Equals(initialNode));

            int fromIndex = initialIndex;
            int toIndex = -1;

            for (int i = 0; i < path.Nodes.Count - 1; i++)
            {
                Node fromNode = path.Nodes[i];
                Node toNode = path.Nodes[i + 1];

                // Find the index of the edge from fromNode to toNode
                toIndex = fromNode.Neighbours.FindIndex(x => x.Key != null && x.Key.Equals(toNode));

                forkliftPath.Add(navigationTable[fromIndex, toIndex]);

                fromIndex = toNode.Neighbours.FindIndex(x => x.Key != null && x.Key.Equals(fromNode));
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
            return "";
        }
    }
}
