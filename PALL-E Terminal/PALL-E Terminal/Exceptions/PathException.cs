using System;

namespace PALL_E_Terminal.Exceptions
{
    public class PathException : System.Exception
    {
        public PathException() : base()
        {
        }

        public PathException(String s) : base(s)
        {
        }

        public PathException(Path p, String s) : this(p.ToString() + s)
        {
        }
    }
}
