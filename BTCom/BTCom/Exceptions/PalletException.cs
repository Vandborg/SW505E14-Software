using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BTCom.Exceptions
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
