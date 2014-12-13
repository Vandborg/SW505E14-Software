using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using BTCom.Exceptions;

namespace BTCom
{
    public class NavigateJob : Job
    {
        public Node Destination { get; set; }
        public string Directions { get; set; }

        public NavigateJob()
        {
            Directions = String.Empty;
            Destination = null;
        }

        private NavigateJob(int identifier) : this()
        {
            Identifier = identifier;
        }

        public NavigateJob(int identifier, String directions) : this(identifier)
        {
            string dir = directions.ToUpper();

            Regex lrsRegex = new Regex("^[LRS]*$");

            if (!lrsRegex.IsMatch(dir))
            {
                throw new Exception("Could not convert '" + Directions + "' to path");
            }

            Directions = dir;
        }

        public NavigateJob(int identifier, Node destination) : this(identifier)
        {
            Destination = destination;
        }

        private static readonly List<List<Char>> navigationMatrix = new List<List<char>>()
        {
            new List<Char>() { '#','L','S','R' }, // 0
            new List<Char>() { 'R','#','L','S' }, // 1
            new List<Char>() { 'S','R','#','L' }, // 2
            new List<Char>() { 'L','S','R','#' }  // 3
        };

        public override Path GetPath()
        {
            if (Directions != String.Empty)
            {
                Forklift f = Database.Instance.Data.Forklifts.FirstOrDefault().Value;

                Node frontNode = f.FrontNode;
                Node rearNode = f.RearNode;

                Path p = new Path();

                foreach (char character in Directions.ToList())
                {
                    int index = frontNode.Neighbours.FindIndex(x => x.Key != null && x.Key.Equals(rearNode));

                    int outIndex = navigationMatrix[index].FindIndex(x => x.Equals(character));

                    Node newFrontNode = frontNode.Neighbours[outIndex].Key;
                    Node newRearNode = frontNode;

                    if (newFrontNode == null)
                    {
                        throw new Exception("Cannot create path with directions '" + Directions + "'. Cannot do '" + character + "' in node '" + frontNode.Name + "' (from '" + rearNode.Name + "')");
                    }

                    p.Nodes.Add(frontNode);

                    frontNode = newFrontNode;
                    rearNode = newRearNode;
                }

                p.Nodes.Add(frontNode);

                if (p.Nodes.Last().Equals(f.FrontNode))
                {
                    throw new JobException("PALL-E already at destination '" + f.FrontNode.Name + "'");
                }

                return p;
            }
            
            if(Destination != null)
            {
                Graph g = Database.Instance.Data.Graphs.FirstOrDefault().Value;

                Forklift f = Database.Instance.Data.Forklifts.FirstOrDefault().Value;

                Node from = f.FrontNode;
                Node ignore = f.RearNode;

                Path p = g.ShortestPath(from, Destination, ignore);

                if (p.Nodes.Last().Equals(f.FrontNode))
                {
                    throw new JobException("Already at destination '" + f.FrontNode.Name + "'");
                }

                return p;
            }

            // Neither directions or destination is set, cannot generate path
            return null;
        }

        public override byte[] GetBytes()
        {
            Forklift f = Database.Instance.Data.Forklifts.FirstOrDefault().Value;
            Node ignore = f.RearNode;

            ForkliftPath fp = new ForkliftPath(GetPath(), ignore);

            return Encoding.ASCII.GetBytes(fp.getDirections());
        }

        public override byte GetJobTypeBytes()
        {
            return BluetoothConnection.TYPE_NAVIGATE_TO;
        }

        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;   // Compare reference to null
            if (obj.GetType() != GetType()) return false;   // Compare types
            if (ReferenceEquals(this, obj)) return true;    // Compare reference to each other

            var other = (NavigateJob) obj;

            // Compare properties of both objects
            var sameIdentifier = this.Identifier == other.Identifier;
            var sameDestination = this.Destination.Equals(other.Destination);
            var sameDirectionns = this.Directions.Equals(other.Directions);

            // Check if all properties are the same
            return sameIdentifier && sameDestination && sameDirectionns;
        }

        public override string ToString()
        {
            String s = "#" + ID() + ". Type: 'navigate'";

            if (Destination != null)
            {
                return s + ". Destination: '" + Destination.Name + "'";
            }
            
            if (Directions != String.Empty)
            {
                return s + ". Directions: '" + Directions.ToUpper() + "'";
            }

            return s;
        }
    }
}
