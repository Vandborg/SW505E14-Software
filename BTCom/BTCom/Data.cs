using System;
using System.Collections.Generic;

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
        public Dictionary<int, Job> Jobs = new Dictionary<int, Job>();
        public Dictionary<int, DebugJob> DebugJobs = new Dictionary<int, DebugJob>();

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

        public void AddJob(Job job)
        {
            try
            {
                Jobs.Add(job.Identifier, job);
            }
            catch (ArgumentException)
            {
                Jobs.Remove(job.Identifier);
                Jobs.Add(job.Identifier, job);
            }
            Database.Instance.Save();
        }

        public bool RemoveJob(Job job)
        {
            bool result = Jobs.Remove(job.Identifier);

            if (result)
            {
                Database.Instance.Save();
            }

            return result;
        }

        public int GetNewJobIdentifier()
        {
            int max = -1;

            foreach (KeyValuePair<int, Job> job in Jobs)
            {
                if (job.Key > max)
                {
                    max = job.Key;
                }
            }

            return max + 1;
        }

        public void AddDebugJob(DebugJob debugJob)
        {
            try
            {
                DebugJobs.Add(debugJob.Identifier, debugJob);
            }
            catch (ArgumentException)
            {
                DebugJobs.Remove(debugJob.Identifier);
                DebugJobs.Add(debugJob.Identifier, debugJob);
            }
            Database.Instance.Save();
        }

        public bool RemoveDebugJob(DebugJob debugJob)
        {
            bool result = DebugJobs.Remove(debugJob.Identifier);

            if (result)
            {
                Database.Instance.Save();
            }

            return result;
        }

        public int GetNewDebugJobIdentifier()
        {
            int max = -1;

            foreach (KeyValuePair<int, DebugJob> debugJob in DebugJobs)
            {
                if (debugJob.Key > max)
                {
                    max = debugJob.Key;
                }
            }

            return max + 1;
        }
    }
}
