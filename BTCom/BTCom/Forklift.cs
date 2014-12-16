using BTCom.Exceptions;

namespace BTCom
{
    public enum Status { UNKNOWN, IDLE, BUSY, OBSTACLE, ERROR }

    public class Forklift : DataEntry
    {
        public Node RearNode { get; set; }
        public Node FrontNode { get; set; }
        public Status Status { get; set; }
        private Pallet _payload;
        public Pallet Payload
        {
            get { return _payload; }
            set
            {
                _payload = value;
                if (_payload != null)
                {
                    _payload.Location = null;
                }
            }
        }

        public bool HasPallet
        {
            get { return Payload != null; }
            set { }
        }

        public Forklift()
        {
            RearNode = null;
            FrontNode = null;
        }

        public bool UpdateNodes(Node frontNode, Node rearNode)
        {
            if (rearNode.Neighbours.FindAll(x => x.Key != null && x.Key.Equals(frontNode)).Count == 0)
            {
                throw new NodeException("Front- and rear-nodes are not neighbours");
            }

            RearNode = rearNode;
            FrontNode = frontNode;

            return true;
        }

        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;   // Compare reference to null
            if (obj.GetType() != GetType()) return false;   // Compare types
            if (ReferenceEquals(this, obj)) return true;    // Compare reference to each other

            var other = (Forklift) obj;

            // Compare properties of both objects
            var sameIdentifier = Identifier == other.Identifier;
            var sameLastNode = RearNode.Equals(other.RearNode);

            // Check if all properties are the same
            return sameIdentifier && sameLastNode;
        }
    }
}
