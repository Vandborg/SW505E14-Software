using System;

namespace PALL_E_Terminal.Exceptions
{
    public class PalletException : Exception
    {
        public PalletException() : base()
        {
        }

        public PalletException(String s) : base(s)
        {
        }

        public PalletException(Pallet n, String s): this(n.ToString() + s)
        {
        }
    }
}
