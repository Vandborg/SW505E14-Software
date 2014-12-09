using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using BTCom.Exceptions;


namespace BTCom
{   
    class Program
    {
        // Main 
        private static void Main(string[] args)
        {
            // Make sure the database is populated
            PopulateDatabase();

            UpdatePositionInformation();

            if(true) // Set to false if you want to communication with NXT 
            {
                // Open the bt-connection
                BluetoothConnection bt = new BluetoothConnection("COM3");

                // Instantiate threads
                Thread ConsoleInputThread = new Thread(() => Commands.Execute());
                Thread ConsumeBTThread = new Thread(() => ConsumeBT(bt));

                // Start the threads
                ConsoleInputThread.Start();
                ConsumeBTThread.Start();            
            }
        }

        // Used to check if position information is required
        private static void UpdatePositionInformation()
        {
            Graph g = Database.Instance.Data.Graphs.FirstOrDefault().Value;
            Forklift f = Database.Instance.Data.Forklifts.FirstOrDefault().Value;
            
            if (f.RearNode == null || f.FrontNode == null)
            {
                Console.WriteLine("Information about PALL-E position is required.");

                bool result = false;

                do
                {
                    Node rearNode = null;
                    Node frontNode = null;

                    // The user must enter a faceing node
                    while (frontNode == null)
                    {
                        try
                        {
                            Console.Write("Front-node: ");
                            String faceing = Console.ReadLine();
                            frontNode = g.getNode(faceing);
                        }
                        catch (NodeException e)
                        {
                            Console.WriteLine("Error when finding node: ");
                            Console.WriteLine(e.Message);
                            Console.WriteLine();
                        }
                    }

                    // The user must enter a last node
                    while (rearNode == null)
                    {
                        try
                        {
                            Console.Write("Rear-node: ");
                            String last = Console.ReadLine();
                            rearNode = g.getNode(last);
                        }
                        catch (NodeException e)
                        {
                            Console.WriteLine("Error when finding node: ");
                            Console.WriteLine(e.Message);
                            Console.WriteLine();
                        }
                    }

                    // Update node information
                    result = f.UpdateNodes(frontNode, rearNode);
                } 
                while (!result);
            }
            else
            {
                Console.WriteLine("PALL-E position information:");
                Console.WriteLine("On the edge between nodes '" + f.RearNode.Name + "' and '" + f.FrontNode.Name + "', facing '" + f.FrontNode.Name + "'");
                Console.WriteLine();
            }
        }

        // Constantly consumes the produces packages from the NXT
        private static void ConsumeBT(BluetoothConnection bt)
        {
            while (true)
            {
                bt.ConsumePackages();
            }
        }

        // Populates the database and ensures all the needed data exists
        private static void PopulateDatabase()
        {
            PopulateDatabaseWithColors();
            PopulateDatabaseWithGraph();
            PopulateDatabaseWithForklift();
        }

        // Populates the database with the forklift
        private static void PopulateDatabaseWithForklift()
        {
            if (Database.Instance.Data.Forklifts.Count <= 0)
            {
                Forklift forklift = new Forklift();
                
                Database.Instance.Data.AddForklift(forklift);
            }
        }

        // Populates the database with the graph
        private static void PopulateDatabaseWithGraph()
        {
            // Check if there is a graph else create a graph
            if (Database.Instance.Data.Graphs.Count <= 0)
            {
                // Instanciate graph
                Graph warehouse = new Graph(1);

                // Instanciate all nodes
                Node A = new Node("A");
                Node B = new Node("B");
                Node C = new Node("C");
                Node D = new Node("D", true);
                Node G = new Node("G");
                Node H = new Node("H");
                Node I = new Node("I", true);
                Node J = new Node("J");
                Node N = new Node("N");
                Node O = new Node("O", true);
                Node P = new Node("P");
                Node Q = new Node("Q");

                // Add the nodes to the graph
                warehouse.AddNode(A);
                warehouse.AddNode(B);
                warehouse.AddNode(C);
                warehouse.AddNode(D);
                warehouse.AddNode(G);
                warehouse.AddNode(H);
                warehouse.AddNode(I);
                warehouse.AddNode(J);
                warehouse.AddNode(N);
                warehouse.AddNode(O);
                warehouse.AddNode(P);
                warehouse.AddNode(Q);

                warehouse.AddUndirectedEdge(new Tuple<Node, int>(A, 1), new Tuple<Node, int>(B, 3), 113);
                warehouse.AddUndirectedEdge(new Tuple<Node, int>(A, 3), new Tuple<Node, int>(P, 0), 100);
                warehouse.AddUndirectedEdge(new Tuple<Node, int>(A, 2), new Tuple<Node, int>(N, 0), 51);

                warehouse.AddUndirectedEdge(new Tuple<Node, int>(B, 1), new Tuple<Node, int>(C, 0), 90);
                warehouse.AddUndirectedEdge(new Tuple<Node, int>(B, 2), new Tuple<Node, int>(H, 0), 51);

                warehouse.AddUndirectedEdge(new Tuple<Node, int>(C, 1), new Tuple<Node, int>(D, 3), 30);
                warehouse.AddUndirectedEdge(new Tuple<Node, int>(C, 2), new Tuple<Node, int>(G, 1), 90);

                warehouse.AddUndirectedEdge(new Tuple<Node, int>(G, 0), new Tuple<Node, int>(H, 2), 50);
                warehouse.AddUndirectedEdge(new Tuple<Node, int>(G, 3), new Tuple<Node, int>(Q, 1), 60);

                warehouse.AddUndirectedEdge(new Tuple<Node, int>(H, 3), new Tuple<Node, int>(J, 1), 60);

                warehouse.AddUndirectedEdge(new Tuple<Node, int>(I, 2), new Tuple<Node, int>(J, 0), 31);

                warehouse.AddUndirectedEdge(new Tuple<Node, int>(J, 3), new Tuple<Node, int>(N, 1), 52);
                warehouse.AddUndirectedEdge(new Tuple<Node, int>(J, 2), new Tuple<Node, int>(Q, 0), 50);

                warehouse.AddUndirectedEdge(new Tuple<Node, int>(N, 2), new Tuple<Node, int>(O, 0), 31);
                warehouse.AddUndirectedEdge(new Tuple<Node, int>(N, 3), new Tuple<Node, int>(P, 1), 51);

                warehouse.AddUndirectedEdge(new Tuple<Node, int>(P, 2), new Tuple<Node, int>(Q, 3), 146);

                // Save it to the database
                Database.Instance.Data.AddGraph(warehouse);
            }
        }

        // Populates the database with colors
        private static void PopulateDatabaseWithColors()
        {
            Database.Instance.Load();
            // Color defines
            const int COLOR_RED_LEFT = 0;
            const int COLOR_RED_RIGHT = 1;
            const int COLOR_BLUE_LEFT = 2;
            const int COLOR_BLUE_RIGHT = 3;
            const int COLOR_BLACK_LEFT = 4;
            const int COLOR_BLACK_RIGHT = 5;
            const int COLOR_GRAY_LEFT = 6;
            const int COLOR_GRAY_RIGHT = 7;
            const int COLOR_WHITE_LEFT = 8;
            const int COLOR_WHITE_RIGHT = 9;

            // The list of all colors when no database exists
            List<Color> colors = new List<Color> 
            { 
                new Color("RED:LEFT", COLOR_RED_LEFT, 484, 122, 129), 
                new Color("RED:RIGHT", COLOR_RED_RIGHT, 535, 225, 206),
                new Color("BLUE:LEFT", COLOR_BLUE_LEFT, 175, 138, 172), 
                new Color("BLUE:RIGHT", COLOR_BLUE_RIGHT, 207, 219, 233), 
                new Color("BLACK:LEFT", COLOR_BLACK_LEFT, 127, 99, 101), 
                new Color("BLACK:RIGHT", COLOR_BLACK_RIGHT, 200, 191, 161), 
                new Color("GRAY:LEFT", COLOR_GRAY_LEFT, 323, 255, 258), 
                new Color("GRAY:RIGHT", COLOR_GRAY_RIGHT, 418, 398, 352), 
                new Color("WHITE:LEFT", COLOR_WHITE_LEFT, 502, 415, 400), 
                new Color("WHITE:RIGHT", COLOR_WHITE_RIGHT, 557, 555, 500)
            };
            
            // Check if the database contains all colors
            foreach (Color c in colors)
            {
                // If the color was not found insert it
                Color color = Database.Instance.Data.Colors.FirstOrDefault(i => i.Value.Identifier == c.Identifier).Value;
                if (color == null)
                {
                    Database.Instance.Data.AddColor(c);
                }
            }
        }
    }
}
