using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SQLite;
using SQLiteNetExtensions.Attributes;


namespace BTCom
{
    class Setting
    {
        [PrimaryKey, AutoIncrement]
        public int Id { get; set; }

        public int BlackRed { get; set; }
        public int BlackGreen { get; set; }
        public int BlackBlue { get; set; }

        public int WhiteRed { get; set; }
        public int WhiteGreen { get; set; }
        public int WhiteBlue { get; set; }
    }
}
