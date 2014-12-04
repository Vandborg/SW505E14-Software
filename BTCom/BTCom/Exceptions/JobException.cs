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

        public JobException(Job j, String s) : this(j.ToString() + s)
        {
        }
    }
}
