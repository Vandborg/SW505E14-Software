using System;
using System.Collections.Generic;
using BTCom;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace BTComTest
{
    [TestClass]
    public class GraphTest
    {
        [TestMethod]
        public void TwoEmptyGraphSameTest()
        {
            Graph g1 = new Graph();
            Graph g2 = new Graph();

            Assert.IsTrue(g1.Equals(g2));
        }

        [TestMethod]
        public void TwoGraphWithSameIdSameTest()
        {
            Graph g1 = new Graph(1);
            Graph g2 = new Graph(1);

            Assert.IsTrue(g1.Equals(g2));
        }

        [TestMethod]
        public void TwoGraphWithDifferentIdSameTest()
        {
            Graph g1 = new Graph(1);
            Graph g2 = new Graph(2);

            Assert.IsFalse(g1.Equals(g2));
        }

        [TestMethod]
        public void TwoGraphWithSameNodeWithoutEdges()
        {
            Graph g1 = new Graph(1);
            Graph g2 = new Graph(2);

            Node n = new Node("n");

            g1.AddNode(n);
            g2.AddNode(n);

            Assert.IsFalse(g1.Equals(g2));
        }

        [TestMethod]
        public void TwoGraphWithEqualNodesWithoutEdges()
        {
            Graph g1 = new Graph(1);
            Graph g2 = new Graph(2);

            Node n1 = new Node("n");
            Node n2 = new Node("n");

            g1.AddNode(n1);
            g2.AddNode(n2);

            Assert.IsFalse(g1.Equals(g2));
        }

        [TestMethod]
        public void TwoGraphWithTwoEqualNodesWithoutEdges()
        {
            Graph g1 = new Graph(1);
            Graph g2 = new Graph(2);

            Node n1 = new Node("n1");
            Node n2 = new Node("n2");
            Node n3 = new Node("n3");
            Node n4 = new Node("n4");

            g1.AddNode(n1);
            g1.AddNode(n2);
            g2.AddNode(n3);
            g2.AddNode(n4);

            Assert.IsFalse(g1.Equals(g2));
        }

        [TestMethod]
        public void TwoGraphWithTwoNodeWithSingleEdge()
        {
            Graph g1 = new Graph(1);
            Graph g2 = new Graph(2);

            Node a = new Node("a");
            Node b = new Node("b");

            Edge ab = new Edge(1);

            g1.AddNode(a);
            g1.AddNode(b);
            g1.AddUndirectedEdge(new Tuple<Node, int>(a, 0), new Tuple<Node, int>(b, 0), ab);

            g2.AddNode(a);
            g2.AddNode(b);
            g2.AddUndirectedEdge(new Tuple<Node, int>(a, 0), new Tuple<Node, int>(b, 0), ab);

            Assert.IsFalse(g1.Equals(g2));
        }

        [TestMethod]
        public void TwoGraphWithThreeNodeWithSingleEdgeOneRemoved()
        {
            Graph g1 = new Graph(1);
            Graph g2 = new Graph(2);

            Node a = new Node("a");
            Node b = new Node("b");
            Node c = new Node("c");

            Edge ab = new Edge(1);
            Edge bc = new Edge(2);

            g1.AddNode(a);
            g1.AddNode(b);
            g1.AddNode(c);
            g1.AddUndirectedEdge(new Tuple<Node, int>(a, 0), new Tuple<Node, int>(b, 0), ab);
            g1.AddUndirectedEdge(new Tuple<Node, int>(b, 1), new Tuple<Node, int>(c, 0), bc);
            g1.RemoveUndirectedEdge(b, c);

            g2.AddNode(a);
            g2.AddNode(b);
            g2.AddNode(c);
            g1.AddUndirectedEdge(new Tuple<Node, int>(a, 0), new Tuple<Node, int>(b, 0), ab);
            g1.AddUndirectedEdge(new Tuple<Node, int>(b, 1), new Tuple<Node, int>(c, 0), bc);
            g2.RemoveUndirectedEdge(b, c);

            Assert.IsFalse(g1.Equals(g2));
        }

        [TestMethod]
        public void TwoGraphWithTwoNodeOneRemoved()
        {
            Graph g1 = new Graph(1);
            Graph g2 = new Graph(2);

            Node a = new Node("a");
            Node b = new Node("b");
            Node c = new Node("b");

            g1.AddNode(a);
            g1.AddNode(b);
            g1.AddNode(c);
            g1.RemoveNode(c);

            g2.AddNode(a);
            g2.AddNode(b);
            g2.AddNode(c);
            g2.RemoveNode(c);

            Assert.IsFalse(g1.Equals(g2));
        }

        [TestMethod]
        public void ShortestPathTwoNodes()
        {
            Graph g = new Graph(1);

            Node a = new Node("a");
            Node b = new Node("b");

            Edge ab = new Edge(10);

            g.AddNode(a);
            g.AddNode(b);

            g.AddUndirectedEdge(new Tuple<Node, int>(a, 0), new Tuple<Node, int>(b, 0), ab);

            Path actual = g.ShortestPath(a, b);

            Path expected = new Path();
            expected.Nodes.Add(a);
            expected.Nodes.Add(b);

            Assert.IsTrue(actual.Equals(expected));
        }

        [TestMethod]
        public void ShortestPathThreeNodes()
        {
            Graph g = new Graph(1);

            Node a = new Node("a");
            Node b = new Node("b");
            Node c = new Node("c");

            Edge ab = new Edge(10);
            Edge bc = new Edge(10);

            g.AddNode(a);
            g.AddNode(b);
            g.AddNode(c);

            g.AddUndirectedEdge(new Tuple<Node, int>(a, 0), new Tuple<Node, int>(b, 0), ab);
            g.AddUndirectedEdge(new Tuple<Node, int>(b, 1), new Tuple<Node, int>(c, 0), bc);

            Path actual = g.ShortestPath(a, c);

            Path expected = new Path();
            expected.Nodes.Add(a);
            expected.Nodes.Add(b);
            expected.Nodes.Add(c);

            Assert.IsTrue(actual.Equals(expected));
        }

        [TestMethod]
        public void ShortestPathThreeNodesTwoPaths()
        {
            Graph g = new Graph(1);

            Node a = new Node("a");
            Node b = new Node("b");
            Node c = new Node("c");

            Edge ab = new Edge(20);
            Edge ac = new Edge(10);
            Edge cb = new Edge(5);

            g.AddNode(a);
            g.AddNode(b);
            g.AddNode(c);

            g.AddUndirectedEdge(new Tuple<Node, int>(a, 0), new Tuple<Node, int>(b, 0), ab);
            g.AddUndirectedEdge(new Tuple<Node, int>(a, 1), new Tuple<Node, int>(c, 0), ac);
            g.AddUndirectedEdge(new Tuple<Node, int>(c, 1), new Tuple<Node, int>(b, 1), cb);

            Path actual = g.ShortestPath(a, b);

            Path expected = new Path();
            expected.Nodes.Add(a);
            expected.Nodes.Add(c);
            expected.Nodes.Add(b);

            Assert.IsTrue(expected.Equals(actual));
        }

        [TestMethod]
        public void ShortestPathTwoNodesNoPaths()
        {
            Graph g = new Graph(1);

            Node a = new Node("a");
            Node b = new Node("b");

            g.AddNode(a);
            g.AddNode(b);

            Path actual = g.ShortestPath(a, b);

            Assert.IsNull(actual);
        }

    }
}
