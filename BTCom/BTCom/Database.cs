using System;
using System.Diagnostics;
using System.Web.Script.Serialization;

namespace BTCom
{
    public sealed class Database
    {
        static Database _instance = null;
        static readonly object padlock = new object();
        private string _databaseName = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments) + "/PALL-E_database.json";

        public string DatabaseName
        {
            get { return _databaseName; }
            set { _databaseName = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments) + "/" + value + ".json" ; }
        }
            
     
        static Database(){}
        private JavaScriptSerializer jsonSerializer = new JavaScriptSerializer();
        private Data _data = new Data();

        public Data Data
        {
            get { return _data; }
            set { _data = value; }
        }

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
            var json = JsonHelper.FormatJson(jsonSerializer.Serialize(Data));
            System.IO.StreamWriter file = new System.IO.StreamWriter(DatabaseName);
            file.Write(json);
            file.Close();
        }

        public void Load()
        {
            string json = System.IO.File.ReadAllText(DatabaseName);
            Data = jsonSerializer.Deserialize<Data>(json);
        }
    }
}
