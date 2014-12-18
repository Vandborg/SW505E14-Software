using System;

namespace PALL_E_Terminal.Exceptions
{
    public class NodeException : System.Exception
    {
        public NodeException() : base()
        {
        }

        public NodeException(String s) : base(s)
        {
        }

        public NodeException(Node n, String s) : this(n.ToString() + s)
        {
        }
    }
}
