using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BTCom
{
    public sealed class Truck
    {
        static Truck _instance = null;
        static readonly object padlock = new object();
        private Node _previousNode = new Node("B");

        static Truck(){ }

        public Node PreviousNode
        {
            get { return _previousNode; }
            set { _previousNode = value; }
        }

        public static Truck Instance
        {
            get
            {
                lock (padlock)
                {
                    if (_instance == null)
                    {
                        _instance = new Truck();
                    }
                    return _instance;
                }
            }
        }
    }
}
