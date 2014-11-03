using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Web.Script.Serialization;

namespace BTCom
{
    public sealed class Database
    {
        static Database _instance = null;
        static readonly object padlock = new object();

        static Database(){}
        private JavaScriptSerializer jsonSerializer = new JavaScriptSerializer();
        public RootObject Data = new RootObject();

        public static Database Instance
        {
            get
            {
                lock (padlock)
                {
                    if(_instance == null)
                    {
                        _instance = new Database();
                    }
                    return _instance;   
                }
            }
        }

        public void Save()
        {
            var json = jsonSerializer.Serialize(_instance);
            Debug.WriteLine(json);
        }

        public void Load()
        {
            string json = "{\"Colors\":[{\"Id\":1,\"Name\":\"red\",\"Red\":20,\"Green\":20,\"Blue\":20}]}";
            Data = jsonSerializer.Deserialize<RootObject>(json);
            Debug.WriteLine(_instance.Data.Colors[0].Blue);
        }
    }
}
