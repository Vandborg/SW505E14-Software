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
            Database.Instance.DebugMode = true;
            // Make sure the database is populated
            PopulateDatabase();

            // Check if there is a graph else create a graph
            if (Database.Instance.Data.Graphs.Count <= 0)
            {
                Graph warehouse = new Graph(1);
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

                warehouse.AddUndirectedEdge(A, B, 1);
                warehouse.AddUndirectedEdge(A, P, 1);
                warehouse.AddUndirectedEdge(A, N, 1);

                warehouse.AddUndirectedEdge(B, C, 1);
                warehouse.AddUndirectedEdge(B, H, 1);

                warehouse.AddUndirectedEdge(C, D, 1);
                warehouse.AddUndirectedEdge(C, F, 1);

                warehouse.AddUndirectedEdge(E, F, 1);

                warehouse.AddUndirectedEdge(F, G, 1);

                warehouse.AddUndirectedEdge(G, H, 1);
                warehouse.AddUndirectedEdge(G, Q, 1);

                warehouse.AddUndirectedEdge(H, J, 1);

                warehouse.AddUndirectedEdge(I, J, 1);

                warehouse.AddUndirectedEdge(J, L, 1);
                warehouse.AddUndirectedEdge(J, Q, 1);

                warehouse.AddUndirectedEdge(K, L, 1);

                warehouse.AddUndirectedEdge(L, M, 1);
                warehouse.AddUndirectedEdge(L, N, 1);

                warehouse.AddUndirectedEdge(N, O, 1);
                warehouse.AddUndirectedEdge(N, P, 1);

                warehouse.AddUndirectedEdge(P, Q, 1);

                Database.Instance.Data.AddGraph(warehouse);
            }

            // Open the bt-connection
            BluetoothConnection bt = new BluetoothConnection("COM3");

            // Instantiate threads
            Thread ConsoleInputThread = new Thread(() => CheckConsoleInput(bt));
            Thread ConsumeBTThread = new Thread(() => ConsumeBT(bt));

            // Start the threads
            ConsoleInputThread.Start();
            ConsumeBTThread.Start();            
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

        // Populates the database
        private static void PopulateDatabase()
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
