using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using BTCom.Exceptions;

namespace BTCom
{
    public class PalletJob : Job
    {
        public Node Destination { get; set; }

        public Pallet PalletLocation { get; set; }

        public PalletJobType Type { get; set; }

        private PalletJob(int identifier, PalletJobType type)
        {
            Destination = null;
            PalletLocation = null;

            Identifier = identifier;
            Type = type;
        }

        public PalletJob(int identifier, Node destination, PalletJobType type) : this(identifier, type)
        {
            Destination = destination;
        }

        public PalletJob(int identifier, Pallet palletLocation, PalletJobType type) : this(identifier, type)
        {
            PalletLocation = palletLocation;
        }

        public override Path GetPath()
        {
            Node target = null;

            if (Destination != null)
            {
                target = Destination;
            }
            else if (PalletLocation != null)
            {
                target = PalletLocation.Location;
            }
            else
            {
                throw new NodeException("Cannot get path because of missing destination or pallet location (both are null)");
            }
            
            Graph g = Database.Instance.Data.Graphs.FirstOrDefault().Value;

            Forklift f = Database.Instance.Data.Forklifts.FirstOrDefault().Value;

            Node from = f.FrontNode;
            Node ignore = f.RearNode;

            Path p = g.ShortestPath(from, target, ignore);

            if (Type == PalletJobType.fetch)
            {
                // Cannot fetch pallets with payload
                if (f.HasPallet)
                {
                    throw new JobException("Cannot fetch pallet with payload");
                }

                // To fetch a pallet from a node, the last node must have a pallet
                if (!p.Nodes.Last().HasPallet)
                {
                    throw new JobException("Cannot fetch pallet where there is none (Node '" + p.Nodes.Last().Name + "')");
                }
            }

            if (Type == PalletJobType.deliver)
            {
                // Cannot deliver without payload
                if (!f.HasPallet)
                {
                    throw new JobException("Cannot deliver pallet without payload");
                }

                if (p.Nodes.Last().HasPallet)
                {
                    throw new JobException("Cannot deliver pallet where there is already one (Node '" + p.Nodes.Last().Name + "')");
                }
            }

            if (p.Nodes.Last().Equals(f.FrontNode))
            {
                throw new JobException("Already at destination '" + f.FrontNode.Name + "'");
            }

            return p;
        }

        public override byte[] GetBytes()
        {
            Forklift f = Database.Instance.Data.Forklifts.FirstOrDefault().Value;
            Node ignore = f.RearNode;

            Path p = GetPath();

            if (p == null)
            {
                throw new NullReferenceException("Path is empty (either at destination, or impossible)");
            }

            ForkliftPath fp = new ForkliftPath(p, ignore);

            string directions = fp.getDirections();

            // We add some special instructions to handle the different job types
            // D - forklift down
            // N - move to next red tape
            // U - forklift up
            // T - Turn around
            // B - Back off small step

            // The instructions is inserted in reversed order
            if (Type == PalletJobType.fetch)
            {
                // Removes the extra N
                directions = directions.Substring(1, directions.Length - 1);

                // Add directions to fetch the pallet
                directions = directions.Insert(0, "TUNDR");
            }
            else if (Type == PalletJobType.deliver)
            {
                // Removes the extra N
                directions = directions.Substring(1, directions.Length - 1);

                // Add directions to deliver the pallet
                directions = directions.Insert(0, "TUBDNR");
            }

            return Encoding.ASCII.GetBytes(directions);
        }

        public override byte GetJobTypeBytes()
        {
            if (Type == PalletJobType.deliver)
            {
                return BluetoothConnection.TYPE_DELIVER_PALLET;
            }
            
            if (Type == PalletJobType.fetch)
            {
                return BluetoothConnection.TYPE_FETCH_PALLET;
            }

            throw new Exception("Cannot determine type of fetch-job");
        }

        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;   // Compare reference to null
            if (obj.GetType() != GetType()) return false;   // Compare types
            if (ReferenceEquals(this, obj)) return true;    // Compare reference to each other

            var other = (PalletJob)obj;

            // Compare properties of both objects
            var sameIdentifier = this.Identifier == other.Identifier;
            var sameType = this.Type.Equals(other.Type);
            var sameDestination = this.Destination.Equals(other.Destination);

            // Check if all properties are the same
            return sameIdentifier && sameType && sameDestination;
        }

        public override string ToString()
        {
            String type = "";

            switch (Type)
            {
                case PalletJobType.deliver:
                    type = "deliver";
                    break;
                case PalletJobType.fetch:
                    type = "fetch";
                    break;
                default:
                    type = "unknown";
                    break;
            }

            if (Destination != null)
            {
                return "#" + ID() + ". Type: '" + type + "'. Destination: '" + Destination.Name + "'";
            }
            
            if(PalletLocation != null)
            {
                return "#" + ID() + ". Type: '" + type + "'. Pallet: '" + PalletLocation.Name + "'";
            }

            return "#" + ID() + ". Type: '" + type + "'";
        }
    }
}
