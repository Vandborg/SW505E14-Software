using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BTCom
{                                                                   
    public abstract class DataEntry
    {
        public int Identifier { get; set; }

        public string ID()
        {
            return Identifier.ToString();
        }
    }
}
