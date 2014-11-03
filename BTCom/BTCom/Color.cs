﻿using System;
using System.Collections.Generic;
using System.Dynamic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BTCom
{
    public class Color : DataEntry
    {
        public string Name { get; set; }
        public int Red { get; set; }
        public int Green { get; set; }
        public int Blue { get; set; }

        public Color(){}
        
        public Color(string name, int id)
        {
            Name = name;
            Identifier = id;
        }

        public Color(string name, int id, int red, int green, int blue) : this(name, id)
        {
            Rgb(red, green, blue);
        }

        public void Rgb(int red, int green, int blue)
        {
            Red = red;
            Green = green;
            Blue = blue;
        }

    }
}
