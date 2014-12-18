using System;

namespace PALL_E_Terminal.Exceptions
{
    public class JobException : Exception
    {
        private Job Job { get; set; }
        private Pallet Pallet { get; set; }

        public JobException() : base()
        {
        }

        public JobException(String s) : base(s)
        {
        }

        public JobException(Job j, String s) : this(s)
        {
            Job = j;
        }

        public JobException(Job j, Pallet p, String s) : this(j, s)
        {
            Pallet = p;
        }
    }
}
