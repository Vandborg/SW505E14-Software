﻿using System;
using BTCom;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace BTComTest
{
    [TestClass]
    public class ColorTest
    {
        [TestMethod]
        public void ColorToRGBBytes()
        {
            Color c = new Color("White", 1, 255, 170, 85);

            byte[] actual = c.ToRGBBytes();
            byte[] expected = new byte[] {50, 53, 53, 49, 55, 48, 48, 56, 53};

            Assert.IsTrue(true);
        }
    }
}
