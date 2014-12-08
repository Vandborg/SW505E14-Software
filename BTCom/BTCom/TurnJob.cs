using System.Text;

namespace BTCom
{
    public class TurnJob : Job
    {
        public TurnJob(int identifier)
        {
            Identifier = identifier;

        }

        public override Path GetPath()
        {
            // This jobs does not consider an actual path, so nothing should be used (aka updated)
            return new Path();
        }

        public override byte[] GetBytes()
        {
            return Encoding.ASCII.GetBytes("T");
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

            var other = (NavigateJob)obj;

            // Compare properties of both objects
            var sameIdentifier = this.Identifier == other.Identifier;

            // Check if all properties are the same
            return sameIdentifier;
        }

        public override string ToString()
        {
            return "#" + ID() + ". Type: 'turn'" + ".";
        }
    }
}
