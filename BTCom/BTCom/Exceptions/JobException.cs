using System;

namespace BTCom.Exceptions
{
    public class JobException : Exception
    {
        public JobException() : base()
        {
        }

        public JobException(String s) : base(s)
        {
        }

        public JobException(Node n, String s) : this(n.ToString() + s)
        {
        }
    }
}
