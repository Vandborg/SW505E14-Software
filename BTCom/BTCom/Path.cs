﻿using System;
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
