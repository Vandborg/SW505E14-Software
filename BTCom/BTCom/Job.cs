using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BTCom
{
    public abstract class Job : DataEntry
    {
        // ReSharper disable once PublicConstructorInAbstractClass
        public Job()
        {
        }

        public abstract Path GetPath();
 
        public abstract byte[] GetBytes();

        public abstract byte GetJobTypeBytes();

        public abstract override string ToString();
    }
}
