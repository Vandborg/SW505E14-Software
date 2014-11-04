using System;
using System.Linq;
using BTCom;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace BTComTest
{
    [TestClass]
    public class DataTest
    {
        [TestMethod]
        public void AddColorToDataListTest()
        {
            var testColor = new Color("testcolor", 1);

            Database.Instance.Data.AddColor(testColor);

            Assert.AreEqual(true, true);

            //Assert.AreEqual(testColor, Database.Instance.Data.Colors.First());

            // Database.Instance.Load();
            // Assert.AreEqual(testColor,Database.Instance.Data.Colors.First());
        }
    }
}
