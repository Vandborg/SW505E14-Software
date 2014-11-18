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
        public Dictionary<int, Color> Colors = new Dictionary<int, Color>();
        public Dictionary<int, Light> Lights = new Dictionary<int, Light>();
        public Dictionary<int, Graph> Graphs = new Dictionary<int, Graph>();
        public Dictionary<int, Path> Paths = new Dictionary<int, Path>();
        public Dictionary<int, Forklift> Forklifts = new Dictionary<int, Forklift>();

        public Data()
        { 
            
        }

        public void AddColor(Color color)
        {
            try
            {
                Colors.Add(color.Identifier, color);
            }
            catch (ArgumentException)
            {
                Colors.Remove(color.Identifier);
                Colors.Add(color.Identifier, color);
            }
            Database.Instance.Save();
        }

        public bool RemoveColor(Color color)
        {
            bool result = Colors.Remove(color.Identifier);
            
            if (result)
            {
                Database.Instance.Save();
            }

            return result;
        }

        public void AddLight(Light light)
        {
            try
            {
                Lights.Add(light.Identifier, light);
            }
            catch (ArgumentException)
            {
                Lights.Remove(light.Identifier);
                Lights.Add(light.Identifier, light);
            }
            Database.Instance.Save();   
        }

        public bool RemoveLight(Light light)
        {
            bool result = Lights.Remove(light.Identifier);

            if (result)
            {
                Database.Instance.Save();
            }

            return result;
        }

        public void AddGraph(Graph graph)
        {
            try
            {
                Graphs.Add(graph.Identifier, graph);
            }
            catch (ArgumentException)
            {
                Graphs.Remove(graph.Identifier);
                Graphs.Add(graph.Identifier, graph);
            }
            Database.Instance.Save();
        }

        public bool RemoveGraph(Graph graph)
        {
            bool result = Graphs.Remove(graph.Identifier);

            if (result)
            {
                Database.Instance.Save();
            }

            return result;
        }

        public void AddPath(Path path)
        {
            try
            {
                Paths.Add(path.Identifier, path);
            }
            catch (ArgumentException)
            {
                Paths.Remove(path.Identifier);
                Paths.Add(path.Identifier, path);
            }
            Database.Instance.Save();
        }

        public bool RemovePath(Path path)
        {
            bool result = Paths.Remove(path.Identifier);

            if (result)
            {
                Database.Instance.Save();
            }

            return result;
        }

        public void AddForklift(Forklift forklift)
        {
            try
            {
                Forklifts.Add(forklift.Identifier, forklift);
            }
            catch (ArgumentException)
            {
                Forklifts.Remove(forklift.Identifier);
                Forklifts.Add(forklift.Identifier, forklift);
            }
            Database.Instance.Save();
        }

        public bool RemoveForklift(Forklift forklift)
        {
            bool result = Forklifts.Remove(forklift.Identifier);

            if (result)
            {
                Database.Instance.Save();
            }

            return result;
        }
    }
}
