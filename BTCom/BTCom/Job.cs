using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;

namespace BTCom
{
    public class Job : DataEntry
    {
        public byte Type { get; set; }
        public Node Destination { get; set; }

        public Job()
        {
            
        }

        public Job(byte type, Node destination)
        {
            Type = type;
            Destination = destination;
        }

        public Job(int identifier, byte type, Node destination) : this(type, destination)
        {
            Identifier = identifier;
        }

        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;   // Compare reference to null
            if (obj.GetType() != GetType()) return false;   // Compare types
            if (ReferenceEquals(this, obj)) return true;    // Compare reference to each other

            var other = (Job) obj;

            // Compare properties of both objects
            var sameIdentifier = this.Identifier == other.Identifier;
            var sameType = this.Type.Equals(other.Type);
            var sameDestination = this.Destination.Equals(other.Destination);

            // Check if all properties are the same
            return sameIdentifier && sameType && sameDestination;
        }

        public Path GetPath()
        {
            Graph g = Database.Instance.Data.Graphs.FirstOrDefault().Value;

            Node from = Database.Instance.Data.Forklifts.FirstOrDefault().Value.FrontNode;
            Node ignore = Database.Instance.Data.Forklifts.FirstOrDefault().Value.RearNode;

            return g.ShortestPath(from, Destination, ignore);
        }

        public byte[] GetBytes(bool printForkliftPath)
        {
            Node ignore = Database.Instance.Data.Forklifts.FirstOrDefault().Value.RearNode;
            
            ForkliftPath fp = new ForkliftPath(this.GetPath(), ignore);

            if (printForkliftPath)
            {
                Console.WriteLine(fp.ToString());
            }

            return Encoding.ASCII.GetBytes(fp.getDirections());
        }

        public override string ToString()
        {
            return "T: '" + ((char) Type).ToString() + "', D: '" + Destination.Name + "'";
        }
    }
}
