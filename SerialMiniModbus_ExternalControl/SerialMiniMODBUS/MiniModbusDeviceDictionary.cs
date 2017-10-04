using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data;
using System.Globalization;
using System.IO;

using System.Runtime.Serialization.Formatters.Binary;
using System.Runtime.Serialization;
using System.Xml.Serialization;

using System.Windows.Forms;



namespace MiniModbus
{
    public delegate string ParameterShow(UInt64 value, uint bitsize);
    public delegate bool ParameterParse(string input, out UInt64 value, uint bitsize);

    static class ConvertFunctions
    {
        static public string ShowInt(UInt64 value, uint bitsize)
        {
            value = value & (((UInt64)1 << (byte)bitsize) - 1);
            if ((value & ((UInt64)1 << ((byte)bitsize - 1))) != 0)
            {
                value = (~value + 1) & (((UInt64)1 << (byte)bitsize) - 1);
                return "-" + value.ToString();
            }
            else
            {
                return value.ToString();
            }
        }
        static public string ShowUInt(UInt64 value, uint bitsize)
        {
            return value.ToString();
        }
        static public string ShowVersion(UInt64 value, uint bitsize)
        {        
            return string.Format("{0}.{1}", (value >> 8) & 0xff, (value >> 0) & 0xff);
        }
        static public string ShowHex(UInt64 value, uint bitsize)
        {
            uint digit_num = (bitsize+3)/4;
            string format = "0x{0:X" + digit_num.ToString() + "}";
            return string.Format(format, value);
        }

        static public string ShowTemperature_x10(UInt64 value, uint bitsize)
        {
            if (value == 0x550)
            {
                return "85°C(неисправность датчика?)";
            }
            else
            {
                double termo = (Int16)value / 16.0;
                string format = "{0:0.0}°C";
                return string.Format(format, termo);
            }
        }
        static public string ShowTemperature_x1(UInt64 value, uint bitsize)
        {
            if (value == 0xff)
            {
                return "Не обнаружен";
            }
            else
            {
                Int32 temp = Convert.ToInt32(value) - 50;
                if (temp == 85)
                {
                    return "85°C(неисправность датчика?)";
                }
                else
                {
                    return string.Format("{0}°C", temp);
                }
            }
        }

        static public string ShowOutputModuleCurrent8bit_x10(UInt64 value, uint bitsize)
        {
            switch (value)
            {
                case 0:
                    return "Выключен(не диагностируется)";
                case 1:
                    return "Короткое замыкание";
                case 2:
                    return "Обрыв"; 
                default:
                    return string.Format("{0:0.0} A", (value - 3) / 10.0);
            }
        }

        static public string ShowOutputModuleCurrent16bit_x100(UInt64 value, uint bitsize)
        {
            switch (value)
            {
                case 0:
                    return "Выключен(не диагностируется)";
                case 1:
                    return "Короткое замыкание";
                case 2:
                    return "Обрыв";
                default:
                    return string.Format("{0:0.00} A", (value - 3) / 100.0);
            }
        }

        static public string ShowTermoId16(UInt64 value, uint bitsize)
        {
            if (value != 0xffff)
                return value.ToString();
            else
                return "Не обнаружен";
        }

        static public string ShowTermoId8(UInt64 value, uint bitsize)
        {
            if (value != 0xff)
                return value.ToString();
            else
                return "Не обнаружен";
        }

        static public string GetShowFuncName(ParameterShow func)
        {
            var res = ParameterShowList.Find((Tuple<string, ParameterShow> t) => { return t.Item2 == func; });
            return res.Item1;
        }

        static public ParameterShow GetShowFunc(string name)
        {
            var res = ParameterShowList.Find((Tuple<string, ParameterShow> t) => { return t.Item1 == name; });
            if (res == null)
            {
                string message = string.Format("Name of function ={0}", name);
                string caption = "Не найдена функция отображения";
                MessageBoxButtons buttons = MessageBoxButtons.OK;
                MessageBox.Show(message, caption, buttons);
                throw new Exception(caption);
            }
            return res.Item2;
        }

        static public string GetParseFuncName(ParameterParse func)
        {
            var res = ParameterParseList.Find((Tuple<string, ParameterParse> t) => { return t.Item2 == func; });
            return res.Item1;
        }

        static public ParameterParse GetParseFunc(string name)
        {
            var res = ParameterParseList.Find((Tuple<string, ParameterParse> t) => { return t.Item1 == name; });
            if (res == null)
            {
                string message = string.Format("Name of function ={0}", name);
                string caption = "Не найдена функция парсинга";
                MessageBoxButtons buttons = MessageBoxButtons.OK;
                MessageBox.Show(message, caption, buttons);
                throw new Exception(caption);
            }
            return res.Item2;
        }

        static List<Tuple<string, ParameterShow>> ParameterShowList;
        static List<Tuple<string, ParameterParse>> ParameterParseList;

        static ConvertFunctions()
        {
            ParameterShowList = new List<Tuple<string, ParameterShow>>();
            ParameterShowList.Add(new Tuple<string, ParameterShow>("ShowInt", ShowInt));
            ParameterShowList.Add(new Tuple<string, ParameterShow>("ShowUInt", ShowUInt));
            ParameterShowList.Add(new Tuple<string, ParameterShow>("ShowVersion", ShowVersion));
            ParameterShowList.Add(new Tuple<string, ParameterShow>("ShowHex1", ShowHex));
            ParameterShowList.Add(new Tuple<string, ParameterShow>("ShowHex2", ShowHex));
            ParameterShowList.Add(new Tuple<string, ParameterShow>("ShowHex3", ShowHex));
            ParameterShowList.Add(new Tuple<string, ParameterShow>("ShowHex4", ShowHex));
            ParameterShowList.Add(new Tuple<string, ParameterShow>("ShowHex", ShowHex));
            ParameterShowList.Add(new Tuple<string, ParameterShow>("ShowTemperature_x10", ShowTemperature_x10));
            ParameterShowList.Add(new Tuple<string, ParameterShow>("ShowTemperature_x1", ShowTemperature_x1));
            ParameterShowList.Add(new Tuple<string, ParameterShow>("ShowTermoId16", ShowTermoId16));
            ParameterShowList.Add(new Tuple<string, ParameterShow>("ShowTermoId8", ShowTermoId8));
            ParameterShowList.Add(new Tuple<string, ParameterShow>("ShowOutputModuleCurrent8bit_x10", ShowOutputModuleCurrent8bit_x10));
            ParameterShowList.Add(new Tuple<string, ParameterShow>("ShowOutputModuleCurrent16bit_x100", ShowOutputModuleCurrent16bit_x100));
            
            ParameterParseList = new List<Tuple<string, ParameterParse>>();
            ParameterParseList.Add(new Tuple<string, ParameterParse>("ParseUInt", ParseUInt));
            ParameterParseList.Add(new Tuple<string, ParameterParse>("ParseUIntHex", ParseUIntHex));
        }


        static public bool ParseUInt(string input, out UInt64 value, uint bitsize)
        {
            return UInt64.TryParse(input, out value);
        }

        static public bool ParseUIntHex(string input, out UInt64 value, uint bitsize)
        {
            string t = input.TrimStart(new char[] { '0', 'x' });
            if (t.Length != 0)
            {
                return UInt64.TryParse(t, NumberStyles.HexNumber, null, out value);
            }
            else
            {
                value = 0;
                return true;
            }
        }

        
    }


    public class DeviceDictionaryROItem 
    {
        public DeviceDictionaryROItem()
        {
            // Empty constructor required to compile.
        }

        public DeviceDictionaryROItem(byte bits, bool use, string name, ParameterShow show)
        {
            Bits = bits;
            Use = use;
            Name = name;
            showFunction = show;
        }

        public static List<DeviceDictionaryROItem> GeneralInfo(string name)
        {
            List<DeviceDictionaryROItem> generalinfo = new List<DeviceDictionaryROItem>();
            generalinfo.Add( new DeviceDictionaryROItem(16, true, "ID устройства", ConvertFunctions.ShowHex));
            generalinfo.Add( new DeviceDictionaryROItem(16, true, "Версия", ConvertFunctions.ShowVersion));
            generalinfo.Add( new DeviceDictionaryROItem(16, true, "hashcode", ConvertFunctions.ShowHex));
            generalinfo.Add( new DeviceDictionaryROItem(16, true, "Размер RO", ConvertFunctions.ShowUInt));
            generalinfo.Add( new DeviceDictionaryROItem(16, true, "Размер RW", ConvertFunctions.ShowUInt));
            generalinfo.Add( new DeviceDictionaryROItem(16, true, "Размер ROM", ConvertFunctions.ShowUInt));
            return generalinfo;
        }

        public byte Bits;
        public bool Use;
        public string Name;
        ParameterShow showFunction;
        public ParameterShow Show() { return showFunction; }
  

        public string ShowFunction
        {
            get
            {
                return ConvertFunctions.GetShowFuncName(showFunction);
            }
            set
            {
                showFunction = ConvertFunctions.GetShowFunc(value);
            }
        }
    }

    public class DeviceDictionaryRWItem : DeviceDictionaryROItem
    {
        public DeviceDictionaryRWItem(byte bits, bool use, string name, ParameterShow show, ParameterParse parse) :
            base(bits, use, name, show)
        {
            parseFunction = parse;
        }

        public DeviceDictionaryRWItem()
        {
            // Empty constructor required to compile.
        }

        public ParameterParse Parse() { return parseFunction; }
        ParameterParse parseFunction;

        public string ParseFunction
        {
            get
            {
                return ConvertFunctions.GetParseFuncName(parseFunction);
            }
            set
            {
                parseFunction = ConvertFunctions.GetParseFunc(value);
            }
        }
    }


    public class DeviceDictionary
    {
        public DeviceDictionary()
        {
        }
        public string Name;
        public List<DeviceDictionaryROItem> GenItems;
        public List<DeviceDictionaryROItem> ROItems;
        public List<DeviceDictionaryRWItem> RWItems;
        public List<DeviceDictionaryRWItem> RWFlashItems;

        public static DeviceDictionary Find(UInt16 id, UInt16 version)
        {
            string dir = Directory.GetCurrentDirectory();
            string[] files = Directory.GetFiles(dir, "*.xml");
            foreach (var file in files)
            {
                string cutted_name = file.Replace(dir, "");
                cutted_name = cutted_name.Replace("\\", "");
                cutted_name = cutted_name.Replace(".xml", "");
                var parts = cutted_name.Split('_');
                if (parts.Length == 3)
                {
                    UInt16[] t = new UInt16[3];
                    t[0] = Convert.ToUInt16(parts[0], 16);
                    t[1] = Convert.ToUInt16(parts[1], 16);
                    t[2] = Convert.ToUInt16(parts[2], 16);

                    if ((id == t[0]) && (version >= t[1]) && (version <= t[2]))
                    {
                        XmlSerializer x = new XmlSerializer(typeof(DeviceDictionary));
                        TextReader reader = new StreamReader(file);
                        DeviceDictionary dictionary = (DeviceDictionary)x.Deserialize(reader);
                        reader.Close();
                        return dictionary;
                    }
                }
            }
            throw new Exception("Нет подходящего файла с описание устройства");
        }
    }


    public class MiniModbusDeviceTableShell
    {
        public event EventHandler TablesWereCreated;

        public MiniModbusDeviceTableShell(MiniModbusDevice device)
        {
            _device = device;
            _device.readingIsComplete += this.readingIsComplete;
            _device.deviceWasChanged += this.deviceWasChanged;
        }

        void deviceWasChanged(object sender, EventArgs args)
        { 
            UInt16[] gendata = _device._gen_data.ReadDeviceData();
            _dictionary = DeviceDictionary.Find(gendata[0], gendata[1]);

            


            GeneralInfo = new ROTableComposition(_device._gen_data, _dictionary.GenItems, "General info");
            ROInfo = new ROTableComposition(_device._RO_data, _dictionary.ROItems, "Read-Only");
            RWInfo = new RWTableComposition(_device._RW_data, _dictionary.RWItems, "Read-Write");
            RWFlashInfo = new RWTableComposition(_device._RW_flash, _dictionary.RWFlashItems, "Read-Write Flash");

            if (TablesWereCreated != null)
            {
                TablesWereCreated(this, null);
            }
        }

        void readingIsComplete(object sender, EventArgs args)
        {
            GeneralInfo.Fill();
            ROInfo.Fill();
            RWInfo.Fill();
            RWFlashInfo.Fill();
        }

        public MiniModbusDevice _device;
        DeviceDictionary _dictionary;
        public string Name { get { return _dictionary.Name; } }

        

        public static UInt64 get_value(UInt16[] data, uint bit_offset, byte bit_size)
        {
            UInt64 mask = ((UInt64)1 << bit_size) - 1;
            UInt64 value = 0;
            byte offset = 0;
            while (bit_size != 0)
            {
                byte l_bit_size = Convert.ToByte( 16 - bit_offset % 16);
                if (l_bit_size > bit_size)
                {
                    l_bit_size = bit_size;
                }
                UInt64 temp = data[bit_offset / 16];
                value |= (temp >> Convert.ToByte(bit_offset % 16)) << offset;
                offset += l_bit_size;
                bit_offset += l_bit_size;
                bit_size -= l_bit_size;
            }
            return value & mask;
        }

        public static void set_value(UInt16[] data, uint bit_offset, byte bit_size, UInt64 value)
        {
            value &= ((UInt64)1 << bit_size) - 1;
            byte offset = 0;

            while (bit_size != 0)
            {
                byte in_word_offset = (byte)(bit_offset % 16);
                byte l_bit_size = (byte)(16 - bit_offset % 16);
                if (l_bit_size > bit_size)
                {
                    l_bit_size = bit_size;
                }
                data[bit_offset / 16] &= (ushort)~(((1 << l_bit_size) - 1) << in_word_offset);
                data[bit_offset / 16] |= (ushort)(((value >> (byte)offset) & (((ulong)1 << l_bit_size) - 1)) << in_word_offset);
                offset += l_bit_size;
                bit_offset += l_bit_size;
                bit_size -= l_bit_size;
            }
        }

        public class ROTableComposition
        {
            public ROTableComposition(MiniModbusDevice.ReadOnlyTable data, List<DeviceDictionaryROItem> items, string name)
            {
                _items = items;
                _data = data;
                Table = new DataTable(name);
                DataColumn column;
                column = new DataColumn();
                column.DataType = System.Type.GetType("System.String");
                column.ColumnName = "Подпись";
                column.ReadOnly = true;
                column.Unique = true;
                // Add the Column to the DataColumnCollection.
                Table.Columns.Add(column);
                // Create new DataColumn, set DataType, 
                // ColumnName and add to DataTable.    
                column = new DataColumn();
                column.DataType = System.Type.GetType("System.String");
                column.ColumnName = "Чтение";
                // Add the Column to the DataColumnCollection.
                Table.Columns.Add(column);

                uint bitsize = 0;
                foreach (var item in items)
                {
                    bitsize += item.Bits;
                }

                if (bitsize%16 != 0)
                {
                    string message = string.Format("Имя таблицы ={0}, рассчитаный размер в битах (по xml файлу ) = {1}, остаток от деления на 16 = {2}", name, bitsize, bitsize % 16);
                    string caption = "Парсинг данных не удался";
                    MessageBoxButtons buttons = MessageBoxButtons.OK;
                    MessageBox.Show(message, caption, buttons);
                    throw new Exception("Парсинг данных не удался из-за неправильного файла .xml");
                }

                if (bitsize != data.Size * 16)
                {
                    string message = string.Format("Имя таблицы ={0}, настоящий размер в битах(по полученной общей информации) = {1}, рассчитаный размер (по xml файлу ) = {2}", name, data.Size * 16, bitsize);
                    string caption = "Парсинг данных не удался";
                    MessageBoxButtons buttons = MessageBoxButtons.OK;
                    MessageBox.Show(message, caption, buttons);
                    throw new Exception("Парсинг данных не удался из-за несовпадения размера в битах");
                }

                foreach (var item in items)
                {
                    if (item.Use)
                    {
                        Table.Rows.Add(item.Name, "");
                    }
                }

                _last_values = new UInt64[Table.Rows.Count];
                was_filled = false;
            }

            public void Fill()
            {
                int table_index = 0;
                uint bit_offset = 0;
                UInt16[] data = _data.ReadDeviceData();
                uint val_index = 0;
                foreach (var item in _items)
                {
                    if (item.Use)
                    {
                        UInt64 val = get_value(data, bit_offset, item.Bits);
                        if ((was_filled == false) || (val != _last_values[val_index]))
                        {
                            Table.Rows[table_index]["Чтение"] = item.Show()(val, item.Bits);
                        }
                        _last_values[val_index] = val;
                        val_index++;
                        table_index++;
                    }
                    bit_offset += item.Bits;
                }

                was_filled = true;
            }

            public DataTable Table;
            MiniModbusDevice.ReadOnlyTable _data;
            List<DeviceDictionaryROItem> _items;
            UInt64[] _last_values;
            bool was_filled;
        }


        public class RWTableComposition
        {
            public RWTableComposition(MiniModbusDevice.ReadWriteTable data, List<DeviceDictionaryRWItem> items, string name)
            {
                _items = items;
                _data = data;
                Table = new DataTable(name);
                DataColumn column;
                column = new DataColumn();
                column.DataType = System.Type.GetType("System.String");
                column.ColumnName = "Подпись";
                column.ReadOnly = true;
                column.Unique = true;
                // Add the Column to the DataColumnCollection.
                Table.Columns.Add(column);
                // Create new DataColumn, set DataType, 
                // ColumnName and add to DataTable.    
                column = new DataColumn();
                column.DataType = System.Type.GetType("System.String");
                column.ColumnName = "Чтение";
                // Add the Column to the DataColumnCollection.
                Table.Columns.Add(column);

                column = new DataColumn();
                column.DataType = System.Type.GetType("System.String");
                column.ColumnName = "Запись";
                // Add the Column to the DataColumnCollection.
                Table.Columns.Add(column);

                uint bitsize = 0;
                foreach (var item in items)
                {
                    bitsize += item.Bits;
                }

                if (bitsize % 16 != 0)
                {
                    string message = string.Format("Имя таблицы ={0}, рассчитаный размер в битах (по xml файлу ) = {1}, остаток от деления на 16 = {2}", name, bitsize, bitsize % 16);
                    string caption = "Парсинг данных не удался";
                    MessageBoxButtons buttons = MessageBoxButtons.OK;
                    MessageBox.Show(message, caption, buttons);
                    throw new Exception("Парсинг данных не удался из-за неправильного файла .xml");
                }

                if (bitsize != data.Size * 16)
                {
                    string message = string.Format("Имя таблицы ={0}, настоящий размер в битах(по полученной общей информации) = {1}, рассчитаный размер (по xml файлу ) = {2}", name, data.Size * 16, bitsize);
                    string caption = "Парсинг данных не удался";
                    MessageBoxButtons buttons = MessageBoxButtons.OK;
                    MessageBox.Show(message, caption, buttons);
                    throw new Exception("Парсинг данных не удался из-за несовпадения размера в битах");
                }

                foreach (var item in items)
                {
                    if (item.Use)
                    {
                        Table.Rows.Add(item.Name, "", "");
                    }
                }

                _last_values = new UInt64[Table.Rows.Count];
                was_filled = false;
            }

            public void Fill()
            {
                int table_index = 0;
                uint bit_offset = 0;
                UInt16[] data = _data.ReadDeviceData();
                UInt16[] to_send = _data.ReadDeviceData();
                uint val_index = 0;
                foreach (var item in _items)
                {
                    if (item.Use)
                    {
                        UInt64 val = get_value(data, bit_offset, item.Bits);
                        if ((was_filled == false) || (val != _last_values[val_index]))
                        {
                            Table.Rows[table_index]["Чтение"] = item.Show()(val, item.Bits);
                        }
                        _last_values[val_index] = val;
                        string value = Table.Rows[table_index]["Запись"] as string;
                        if ( (value != null) && (value != ""))
                        {
                            if (item.Parse()(value, out val, item.Bits))
                            {
                                set_value(to_send, bit_offset, item.Bits, val);
                            }
                            else
                            {
                                //вызвать событие?
                            }
                        }

                        val_index++;
                        table_index++;
                    }
                    bit_offset += item.Bits;
                }
                _data.WriteDeviceData(to_send);
                was_filled = true;
            }

            public DataTable Table;
            MiniModbusDevice.ReadWriteTable _data;
            List<DeviceDictionaryRWItem> _items;
            UInt64[] _last_values;
            bool was_filled;
        }





        public ROTableComposition GeneralInfo;
        public ROTableComposition ROInfo;
        public RWTableComposition RWInfo;
        public RWTableComposition RWFlashInfo;
    }

}
