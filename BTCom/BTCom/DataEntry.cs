using System;

namespace BTCom
{                                                                   
    public abstract class DataEntry
    {
        public int Identifier { get; set; }

        public string ID()
        {
            return Identifier.ToString();
        }

        public new abstract bool Equals(Object obj);

        public override string ToString()
        {
            return base.ToString() + " - " +  "Identifier: '" + Identifier + "'.";
        }
    }
}
