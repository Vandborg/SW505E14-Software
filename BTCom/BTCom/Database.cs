using Newtonsoft.Json;
using System;
using System.Diagnostics;
//using System.Web.Script.Serialization;

namespace BTCom
{
    public sealed class Database
    {
        static Database _instance = null;
        static readonly object padlock = new object();
        private string _databaseName = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments) + "/PALL-E_database.json";

        private bool _debugMode = false;
        public bool DebugMode
        {
            get { return _debugMode; }
            set { _debugMode = value; }
        }

        public string DatabaseName
        {
            get { return _databaseName; }
            set { _databaseName = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments) + "/" + value + ".json"; }
        }

        static Database() { }
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
                    if (_instance == null)
                    {
                        _instance = new Database();
                        _instance.Load();
                    }
                    return _instance;
                }
            }
        }

        public void Save()
        {
            var json = JsonConvert.SerializeObject(Data, Formatting.None, new JsonSerializerSettings { ReferenceLoopHandling = ReferenceLoopHandling.Ignore });

            if (DebugMode)
            {
                json = JsonHelper.FormatJson(json);
            }

            System.IO.StreamWriter file = new System.IO.StreamWriter(DatabaseName);
            file.Write(json);
            file.Close();
        }

        public void Load()
        {
            // Make sure that the file exists
            if (!System.IO.File.Exists((DatabaseName)))
            {
                Save();
            }

            string json = System.IO.File.ReadAllText(DatabaseName);
            
            Data = JsonConvert.DeserializeObject<Data>(json);
        }
    }
}
