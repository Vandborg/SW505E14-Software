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

        private static string[,] navigationTable = new string[4, 4] { {"","L","F","R"}, {"R","","L","F"},
                                                                      {"F","R","","L"}, {"L","F","R",""} };
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

        public string[] convertToCarPath()
        {
            var carPath = new string[Nodes.Count];
            //This is not working, but got the gist of it.
            /*KeyValuePair<Node, Edge> previousNodeKeyvalue = new KeyValuePair<Node, Edge>();

            for (int i = 0; i < Nodes.Count-1; i++)
            {
                int comingFromNodeIndex;
                var nodeOne = Nodes[i];
                var nodeTwo = Nodes[i+1];
                //Console.WriteLine(nodeOne);
                //Console.WriteLine(nodeTwo);
                var nodeOneKeyvalue = nodeOne.Neighbours.Single(x => nodeTwo.Equals(x.Key));
                var nodeTwoKeyvalue = nodeTwo.Neighbours.Single(x => nodeOne.Equals(x.Key));

                if (previousNodeKeyvalue.Key == null)
                {
                    comingFromNodeIndex = nodeOne.Neighbours.IndexOf(nodeOneKeyvalue);
                }
                else
                {
                    comingFromNodeIndex = nodeOne.Neighbours.IndexOf(previousNodeKeyvalue);
                }

                previousNodeKeyvalue = nodeOneKeyvalue;

                Console.WriteLine(nodeOne.Neighbours.IndexOf(nodeOneKeyvalue) + ", " + nodeTwo.Neighbours.IndexOf(nodeTwoKeyvalue));
                carPath[i] =
                    navigationTable[
                        comingFromNodeIndex, nodeTwo.Neighbours.IndexOf(nodeTwoKeyvalue)];
            }*/

            return carPath;
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
