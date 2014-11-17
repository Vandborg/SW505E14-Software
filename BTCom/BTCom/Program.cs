using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;


namespace BTCom
{   
    class Program
    {
        // Main 
        private static void Main(string[] args)
        {
            // Make sure the database is populated
            PopulateDatabase();            

            if(true) // Set to false if you want to communication with NXT 
            {
                // Open the bt-connection
                BluetoothConnection bt = new BluetoothConnection("COM6");
                bt.FetchPallet(new Path());

                // Instantiate threads
                Thread ConsoleInputThread = new Thread(() => CheckConsoleInput(bt));
                Thread ConsumeBTThread = new Thread(() => ConsumeBT(bt));

                // Start the threads
                ConsoleInputThread.Start();
                ConsumeBTThread.Start();            
            }
        }

        // Constantly checks console input
        private static void CheckConsoleInput(BluetoothConnection bt)
        {
            while (true)
            {
                bt.CreateJob(Console.ReadLine());
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
                Node D = new Node("D");
                Node E = new Node("E");
                Node F = new Node("F");
                Node G = new Node("G");
                Node H = new Node("H");
                Node I = new Node("I");
                Node J = new Node("J");
                Node K = new Node("K");
                Node L = new Node("L");
                Node M = new Node("M");
                Node N = new Node("N");
                Node O = new Node("O");
                Node P = new Node("P");
                Node Q = new Node("Q");

                // Add the nodes to the graph
                warehouse.AddNode(A);
                warehouse.AddNode(B);
                warehouse.AddNode(C);
                warehouse.AddNode(D);
                warehouse.AddNode(E);
                warehouse.AddNode(F);
                warehouse.AddNode(G);
                warehouse.AddNode(H);
                warehouse.AddNode(I);
                warehouse.AddNode(J);
                warehouse.AddNode(K);
                warehouse.AddNode(L);
                warehouse.AddNode(M);
                warehouse.AddNode(N);
                warehouse.AddNode(O);
                warehouse.AddNode(P);
                warehouse.AddNode(Q);

                warehouse.AddUndirectedEdge(new Tuple<Node, int>(A, 1), new Tuple<Node, int>(B, 3), 4);
                warehouse.AddUndirectedEdge(new Tuple<Node, int>(A, 3), new Tuple<Node, int>(P, 0), 2);
                warehouse.AddUndirectedEdge(new Tuple<Node, int>(A, 2), new Tuple<Node, int>(N, 0), 1);

                warehouse.AddUndirectedEdge(new Tuple<Node, int>(B, 1), new Tuple<Node, int>(C, 0), 2);
                warehouse.AddUndirectedEdge(new Tuple<Node, int>(B, 2), new Tuple<Node, int>(H, 0), 1);

                warehouse.AddUndirectedEdge(new Tuple<Node, int>(C, 1), new Tuple<Node,int>(D, 3), 1);
                warehouse.AddUndirectedEdge(new Tuple<Node, int>(C, 2), new Tuple<Node, int>(F, 0), 1);

                warehouse.AddUndirectedEdge(new Tuple<Node, int>(E, 3), new Tuple<Node, int>(F, 1), 1);

                warehouse.AddUndirectedEdge(new Tuple<Node, int>(F, 2), new Tuple<Node, int>(G, 1), 2);

                warehouse.AddUndirectedEdge(new Tuple<Node, int>(G, 0), new Tuple<Node, int>(H, 2), 1);
                warehouse.AddUndirectedEdge(new Tuple<Node, int>(G, 3), new Tuple<Node, int>(Q, 1), 1);

                warehouse.AddUndirectedEdge(new Tuple<Node, int>(H, 3), new Tuple<Node, int>(J, 1), 1);

                warehouse.AddUndirectedEdge(new Tuple<Node, int>(I, 2), new Tuple<Node,int>(J, 0), 1);

                warehouse.AddUndirectedEdge(new Tuple<Node, int>(J, 3), new Tuple<Node, int>(L, 1), 1);
                warehouse.AddUndirectedEdge(new Tuple<Node, int>(J, 2), new Tuple<Node,int>(Q, 0), 1);

                warehouse.AddUndirectedEdge(new Tuple<Node, int>(K, 2), new Tuple<Node,int>(L, 0), 1);

                warehouse.AddUndirectedEdge(new Tuple<Node, int>(L, 2), new Tuple<Node,int>(M, 0), 1);
                warehouse.AddUndirectedEdge(new Tuple<Node, int>(L, 3), new Tuple<Node,int>(N, 1), 1);

                warehouse.AddUndirectedEdge(new Tuple<Node, int>(N, 2), new Tuple<Node,int>(O, 0), 1);
                warehouse.AddUndirectedEdge(new Tuple<Node, int>(N, 3), new Tuple<Node,int>(P, 1), 1);

                warehouse.AddUndirectedEdge(new Tuple<Node, int>(P, 2), new Tuple<Node, int>(Q, 3), 4);

                // Save it to the database
                Database.Instance.Data.AddGraph(warehouse);
            }
        }

        // Populates the database with colors
        private static void PopulateDatabaseWithColors()
        {
            Database.Instance.Load();
            // Color defines
            const int COLOR_RED_LEFT = 1;
            const int COLOR_RED_RIGHT = 2;
            const int COLOR_BLUE_LEFT = 3;
            const int COLOR_BLUE_RIGHT = 4;
            const int COLOR_BLACK_LEFT = 5;
            const int COLOR_BLACK_RIGHT = 6;
            const int COLOR_GRAY_LEFT = 7;
            const int COLOR_GRAY_RIGHT = 8;
            const int COLOR_WHITE_LEFT = 9;
            const int COLOR_WHITE_RIGHT = 10;

            // The list of all colors when no database exists
            List<Color> colors = new List<Color> 
            { 
                new Color("RED:LEFT", COLOR_RED_LEFT, 315, 115, 117), 
                new Color("RED:RIGHT", COLOR_RED_RIGHT, 365, 200, 178),
                new Color("BLUE:LEFT", COLOR_BLUE_LEFT, 175, 138, 172), 
                new Color("BLUE:RIGHT", COLOR_BLUE_RIGHT, 207, 219, 233), 
                new Color("BLACK:LEFT", COLOR_BLACK_LEFT, 173, 128, 125), 
                new Color("BLACK:RIGHT", COLOR_BLACK_RIGHT, 200, 295, 263), 
                new Color("GRAY:LEFT", COLOR_GRAY_LEFT, 323, 255, 258), 
                new Color("GRAY:RIGHT", COLOR_GRAY_RIGHT, 418, 398, 352), 
                new Color("WHITE:LEFT", COLOR_WHITE_LEFT, 427, 458, 351), 
                new Color("WHITE:RIGHT", COLOR_WHITE_RIGHT, 500, 488, 437)
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
