using System;
using System.Collections.Generic;
using System.Linq;

namespace BTCom
{
    public class Data
    {
        // Keys have to be strings in Dictionaries to be serializable to json
        public Dictionary<int, Color> Colors = new Dictionary<int, Color>();
        public Dictionary<int, Graph> Graphs = new Dictionary<int, Graph>();
        public Dictionary<int, Path> Paths = new Dictionary<int, Path>();
        public Dictionary<int, Forklift> Forklifts = new Dictionary<int, Forklift>();
        public Dictionary<int, Job> Jobs = new Dictionary<int, Job>();
        public Dictionary<int, Pallet> Pallets = new Dictionary<int, Pallet>();

        // Identifiers for jobs and pallets
        private int lastJobIdentifier;
        private int lastPalletIdentifier;

        public Data()
        {
            lastJobIdentifier = Jobs.Count > 0 ? Jobs.Keys.Max() : 0;
            lastJobIdentifier = Pallets.Count > 0 ? Pallets.Keys.Max() : 0;
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
        }

        public bool RemoveColor(Color color)
        {
            bool result = Colors.Remove(color.Identifier);
            
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
        }

        public bool RemoveGraph(Graph graph)
        {
            bool result = Graphs.Remove(graph.Identifier);
            
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
        }

        public bool RemovePath(Path path)
        {
            bool result = Paths.Remove(path.Identifier);
            
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
        }

        public bool RemoveForklift(Forklift forklift)
        {
            bool result = Forklifts.Remove(forklift.Identifier);
            
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
        }

        public bool RemoveJob(Job job)
        {
            bool result = Jobs.Remove(job.Identifier);

            return result;
        }

        public int GetNewJobIdentifier()
        {
            int max = lastJobIdentifier;

            foreach (KeyValuePair<int, Job> job in Jobs)
            {
                if (job.Key > max)
                {
                    max = job.Key;
                }
            }

            lastJobIdentifier = max + 1;

            return max + 1;
        }

        public void AddPallet(Pallet pallet)
        {
            try
            {
                Pallets.Add(pallet.Identifier, pallet);
            }
            catch (ArgumentException)
            {
                Pallets.Remove(pallet.Identifier);
                Pallets.Add(pallet.Identifier, pallet);
            }
        }

        public bool RemovePallet(Pallet pallet)
        {
            bool result = Pallets.Remove(pallet.Identifier);

            return result;
        }

        public int GetNewPalletIdentifier()
        {
            int max = lastPalletIdentifier;

            foreach (KeyValuePair<int, Pallet> pallet in Pallets)
            {
                if (pallet.Key > max)
                {
                    max = pallet.Key;
                }
            }

            lastPalletIdentifier = max + 1;

            return max + 1;
        }
    }
}
