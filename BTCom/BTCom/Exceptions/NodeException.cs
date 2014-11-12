using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BTCom.Exceptions
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
