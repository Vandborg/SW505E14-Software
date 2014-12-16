﻿using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using BTCom.Exceptions;


namespace BTCom
{   
    class Program
    {
        public static bool HasConnection = false;

        // Main 
        private static void Main(string[] args)
        {
            Console.Title = ConsoleHandler.DNS + " console";
            Console.WindowWidth = 120;
            Console.WindowHeight = 60;

            Thread ConsoleThread = new Thread(() => ConsoleHandler.PrintMessages());
            ConsoleThread.Start();

            ConsoleHandler.AddMessage(MessageType.SUCCESS, "______    ___    _       _                _____ ");
            ConsoleHandler.AddMessage(MessageType.SUCCESS, "| ___ \\  / _ \\  | |     | |              |  ___|");
            ConsoleHandler.AddMessage(MessageType.SUCCESS, "| |_/ / / /_\\ \\ | |     | |      ______  | |__  ");
            ConsoleHandler.AddMessage(MessageType.SUCCESS, "|  __/  |  _  | | |     | |     |______| |  __| ");
            ConsoleHandler.AddMessage(MessageType.SUCCESS, "| |     | | | | | |____ | |____          | |___ ");
            ConsoleHandler.AddMessage(MessageType.SUCCESS, "\\_|     \\_| |_/ \\_____/ \\_____/          \\_____|");
            ConsoleHandler.AddMessage(MessageType.SUCCESS, "");

            // Make sure the database is populated
            PopulateDatabase();

            // Test and set if we know where PALL-E is
            UpdatePositionInformation();
            
            // Instantiate threads
            Thread ConsumeBTThread = new Thread(() => ConsumeBT(new BluetoothConnection("COM3")));
            
            // Start the threads
            ConsumeBTThread.Start();

            // Wait until the device has bluetooth
            while (!HasConnection)
            {
                ; // Busy wait
            }

            Thread CommandThread = new Thread(() => Commands.Execute());
            CommandThread.Start();          
        }

        // Used to check if position information is required
        public static void UpdatePositionInformation()
        {
            Graph g = Database.Instance.Data.Graphs.FirstOrDefault().Value;
            Forklift f = Database.Instance.Data.Forklifts.FirstOrDefault().Value;
            
            if (f.RearNode == null || f.FrontNode == null)
            {
                ConsoleHandler.AddMessage(MessageType.REGULAR, "Information about PALL-E position is required.");

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
                            ConsoleHandler.AddMessage(MessageType.REGULAR, "Enter name of front node...");
                            String faceing = Console.ReadLine();
                            ConsoleHandler.ClearCommand();
                            frontNode = g.getNode(faceing);
                        }
                        catch (NodeException e)
                        {
                            ConsoleHandler.AddMessage(MessageType.ERROR, "Error when finding node: '" + e.Message + "'");
                        }
                    }

                    // The user must enter a last node
                    while (rearNode == null)
                    {
                        try
                        {
                            ConsoleHandler.AddMessage(MessageType.REGULAR, "Enter name of rear node...");
                            String last = Console.ReadLine();
                            ConsoleHandler.ClearCommand();
                            rearNode = g.getNode(last);
                        }
                        catch (NodeException e)
                        {
                            ConsoleHandler.AddMessage(MessageType.ERROR, "Error when finding node: '" + e.Message + "'");
                        }
                    }

                    // Update node information
                    result = f.UpdateNodes(frontNode, rearNode);

                    Commands.PrintSuccess("Position updated");
                    Commands.Position();
                } 
                while (!result);
            }
            else
            {
                ConsoleHandler.AddMessage(MessageType.REGULAR, ConsoleHandler.DNS + " position information:");
                ConsoleHandler.AddMessage(MessageType.REGULAR, "Front-node: '" + f.FrontNode.Name + "'. Rear-node: '" + f.RearNode.Name + "'");
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
            Commands.Save();
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

                warehouse.AddUndirectedEdge(new Tuple<Node, int>(C, 1), new Tuple<Node, int>(D, 3), 43);
                warehouse.AddUndirectedEdge(new Tuple<Node, int>(C, 2), new Tuple<Node, int>(G, 1), 90);

                warehouse.AddUndirectedEdge(new Tuple<Node, int>(G, 0), new Tuple<Node, int>(H, 2), 50);
                warehouse.AddUndirectedEdge(new Tuple<Node, int>(G, 3), new Tuple<Node, int>(Q, 1), 60);

                warehouse.AddUndirectedEdge(new Tuple<Node, int>(H, 3), new Tuple<Node, int>(J, 1), 60);

                warehouse.AddUndirectedEdge(new Tuple<Node, int>(I, 2), new Tuple<Node, int>(J, 0), 41);

                warehouse.AddUndirectedEdge(new Tuple<Node, int>(J, 3), new Tuple<Node, int>(N, 1), 52);
                warehouse.AddUndirectedEdge(new Tuple<Node, int>(J, 2), new Tuple<Node, int>(Q, 0), 50);

                warehouse.AddUndirectedEdge(new Tuple<Node, int>(N, 2), new Tuple<Node, int>(O, 0), 41);
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
                new Color("RED:LEFT", COLOR_RED_LEFT, 487, 147, 149), 
                new Color("RED:RIGHT", COLOR_RED_RIGHT, 538, 235, 210),
                new Color("BLUE:LEFT", COLOR_BLUE_LEFT, 175, 138, 172), 
                new Color("BLUE:RIGHT", COLOR_BLUE_RIGHT, 207, 219, 233), 
                new Color("BLACK:LEFT", COLOR_BLACK_LEFT, 145, 114, 115), 
                new Color("BLACK:RIGHT", COLOR_BLACK_RIGHT, 204, 199, 172), 
                new Color("GRAY:LEFT", COLOR_GRAY_LEFT, 323, 255, 258), 
                new Color("GRAY:RIGHT", COLOR_GRAY_RIGHT, 418, 398, 352), 
                new Color("WHITE:LEFT", COLOR_WHITE_LEFT, 505, 418, 402), 
                new Color("WHITE:RIGHT", COLOR_WHITE_RIGHT, 554, 548, 493)
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
