using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using BTCom.Exceptions;

namespace BTCom
{
    public class NavigateJob : Job
    {
        public Node Destination { get; set; }

        public NavigateJob()
        {
            Destination = null;
        }

        private NavigateJob(int identifier) : this()
        {
            Identifier = identifier;
        }

        public NavigateJob(int identifier, Node destination) : this(identifier)
        {
            Destination = destination;
        }

        public override Path GetPath()
        {
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

            // Check if all properties are the same
            return sameIdentifier && sameDestination;
        }

        public override string ToString()
        {
            String s = "#" + ID() + ". Type: 'navigate'";

            if (Destination != null)
            {
                return s + ". Destination: '" + Destination.Name + "'";
            }

            return s;
        }
    }
}
