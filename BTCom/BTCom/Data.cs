using System;
using System.Collections;
using System.Collections.Generic;
using System.Data.Entity.Core.Objects;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Web.Script.Serialization;

namespace BTCom
{
    public class Data
    {
        //Keys have to be strings in Dictionaries to be serializable to json
        public Dictionary<String, Color> Colors = new Dictionary<String, Color>();

        public Data()
        { 
            
        }

        public void AddColor(Color color)
        {
            try
            {
                Colors.Add(color.ID(), color);
            }
            catch (ArgumentException)
            {
                Colors.Remove(color.ID());
                Colors.Add(color.ID(), color);
            }
            Database.Instance.Save();
        }

        public bool RemoveColor(Color color)
        {
            bool result = Colors.Remove(color.ID());
            
            if (result)
            {
                Database.Instance.Save();
            }

            return result;
        }
    }
}
