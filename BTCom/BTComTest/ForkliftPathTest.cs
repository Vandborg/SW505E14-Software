using System;
using System.Collections.Generic;
using System.Linq;
using BTCom;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace BTComTest
{
    [TestClass]
    public class ForkliftPathTest
    {
        [TestMethod]
        public void TestForkliftPathGraphSixNodesFromAToBBackToB()
        {
            Graph g = Database.Instance.Data.Graphs.FirstOrDefault().Value;

            Path p = g.ShortestPath(g.getNode("a"), g.getNode("b"), g.getNode("b"));

            ForkliftPath fp = new ForkliftPath(p, g.getNode("b"));

            string actual = fp.getDirections();
            string expected = "LSSLL";

            Assert.IsTrue(actual == expected);

        }

        [TestCleanup()]
        public void Cleanup()
        {
            // Make sure that the database is clean
            cleanDatabase();
        }

        [TestInitialize()]
        public void Initialize()
        {
            // Make the database use a testing-database
            Database.Instance.DatabaseName = "PALL-E-Database-Test";

            // Make sure that the database is clean
            cleanDatabase();

            PopulateDatabaseWithGraph();
        }

        private void cleanDatabase()
        {
            // Clean the database after each test
            Database.Instance.Data.Graphs = new Dictionary<int, Graph>();
            
            // Save database
            Database.Instance.Save();
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

                warehouse.AddUndirectedEdge(new Tuple<Node, int>(C, 1), new Tuple<Node, int>(D, 3), 1);
                warehouse.AddUndirectedEdge(new Tuple<Node, int>(C, 2), new Tuple<Node, int>(F, 0), 1);

                warehouse.AddUndirectedEdge(new Tuple<Node, int>(E, 3), new Tuple<Node, int>(F, 1), 1);

                warehouse.AddUndirectedEdge(new Tuple<Node, int>(F, 2), new Tuple<Node, int>(G, 1), 2);

                warehouse.AddUndirectedEdge(new Tuple<Node, int>(G, 0), new Tuple<Node, int>(H, 2), 1);
                warehouse.AddUndirectedEdge(new Tuple<Node, int>(G, 3), new Tuple<Node, int>(Q, 1), 1);

                warehouse.AddUndirectedEdge(new Tuple<Node, int>(H, 3), new Tuple<Node, int>(J, 1), 1);

                warehouse.AddUndirectedEdge(new Tuple<Node, int>(I, 2), new Tuple<Node, int>(J, 0), 1);

                warehouse.AddUndirectedEdge(new Tuple<Node, int>(J, 3), new Tuple<Node, int>(L, 1), 1);
                warehouse.AddUndirectedEdge(new Tuple<Node, int>(J, 2), new Tuple<Node, int>(Q, 0), 1);

                warehouse.AddUndirectedEdge(new Tuple<Node, int>(K, 2), new Tuple<Node, int>(L, 0), 1);

                warehouse.AddUndirectedEdge(new Tuple<Node, int>(L, 2), new Tuple<Node, int>(M, 0), 1);
                warehouse.AddUndirectedEdge(new Tuple<Node, int>(L, 3), new Tuple<Node, int>(N, 1), 1);

                warehouse.AddUndirectedEdge(new Tuple<Node, int>(N, 2), new Tuple<Node, int>(O, 0), 1);
                warehouse.AddUndirectedEdge(new Tuple<Node, int>(N, 3), new Tuple<Node, int>(P, 1), 1);

                warehouse.AddUndirectedEdge(new Tuple<Node, int>(P, 2), new Tuple<Node, int>(Q, 3), 4);

                // Save it to the database
                Database.Instance.Data.AddGraph(warehouse);
            }
        }
    }
}
