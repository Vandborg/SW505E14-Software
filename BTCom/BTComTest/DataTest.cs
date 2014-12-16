using System;
using System.Collections.Generic;
using System.Linq;
using BTCom;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace BTComTest
{
    [TestClass]
    public class DataTest
    {
        [TestMethod]
        public void AddColorToDataListEmptyConstructorTest()
        {
            var expected = new Color();
           
            Database.Instance.Data.AddColor(expected);

            var actual = Database.Instance.Data.Colors.First().Value;

            Assert.AreEqual(actual, expected);
        }

        [TestMethod]
        public void AddColorToDataListNameAndIdentifierConstructorTest()
        {
            var expected = new Color("testcolor", 1);

            Database.Instance.Data.AddColor(expected);

            var actual = Database.Instance.Data.Colors.First().Value;

            Assert.IsTrue(expected.Equals(actual));
        }

        [TestMethod]
        public void AddColorToDataListFullConstructorTest()
        {
            var expected = new Color("testcolor", 1, 1, 2, 3);

            Database.Instance.Data.AddColor(expected);

            var actual = Database.Instance.Data.Colors.First().Value;

            Assert.IsTrue(expected.Equals(actual));
        }

        [TestMethod]
        public void AddColorToDatabaseEmptyConstructorTest()
        {
            var expected = new Color();
            
            Database.Instance.Data.AddColor(expected);
            Database.Instance.Save();
            Database.Instance.Load();

            var actual = Database.Instance.Data.Colors.First().Value;

            Assert.IsTrue(expected.Equals(actual));
        }

        [TestMethod]
        public void AddColorToDatabaseNameAndIdentifierConstructorTest()
        {
            var expected = new Color("testcolor", 1);

            Database.Instance.Data.AddColor(expected);
            Database.Instance.Save();
            Database.Instance.Load();

            var actual = Database.Instance.Data.Colors.First().Value;

            Assert.IsTrue(expected.Equals(actual));
        }

        [TestMethod]
        public void AddColorToDatabaseFullConstructorTest()
        {
            var expected = new Color("testcolor", 1, 1, 2, 3);

            Database.Instance.Data.AddColor(expected);
            Database.Instance.Save();
            Database.Instance.Load();

            var actual = Database.Instance.Data.Colors.First().Value;

            Assert.IsTrue(expected.Equals(actual));
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
        }

        private void cleanDatabase()
        {
            // Clean the database after each test
            Database.Instance.Data.Colors = new Dictionary<int, Color>();
            Database.Instance.Data.Graphs = new Dictionary<int, Graph>();

            // Save database
            Database.Instance.Save();
        }
    }
}
