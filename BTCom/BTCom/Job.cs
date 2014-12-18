namespace PALL_E_Terminal
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
