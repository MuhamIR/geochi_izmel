using System;
using System.Collections.Generic;
using System.Text;
using System.Globalization;

namespace AddUtility
{
    public static class AddUtils
    {
        public static byte[] ConvertToSend(string stringtoconvert)
        {
            Encoding encode = Encoding.GetEncoding(1251);
            bool in_spec = false;
            string spec_s = "";
            List<byte> list = new List<byte>();
            foreach (char c in stringtoconvert)
            {
                if (in_spec)
                {
                    if (c == '}')
                    {
                        byte[] bb = convertSpecStringToSend(spec_s);
                        if (bb == null)
                            return null;
                        else
                        {
                            list.AddRange(bb);
                        }
                        spec_s = "";
                        in_spec = false;
                    }
                    else
                    {
                        spec_s += c;
                    }
                }
                else
                {
                    if (c == '{')
                    {
                        spec_s = "";
                        in_spec = true;
                    }
                    else
                    {
                        string t = new string(c, 1);
                        var b = encode.GetBytes(t);
                        list.Add(b[0]);
                    }
                }
            }

            if(in_spec || (list.Count == 0))
                return null;
            else
                return list.ToArray();
        }

        static byte[] convertSpecStringToSend(string specstring)
        {
            var provider = CultureInfo.InvariantCulture;
            string[] parts = specstring.Split(':');
            if (parts.Length == 1)
            {
                byte number;
                bool result = byte.TryParse(parts[0], out number);
                if (result)
                {
                    return new byte[1] { number };
                }
                else
                    return null;
            }
            else if (parts.Length == 2)
            {
                parts[0] = parts[0].Trim();
                parts[1] = parts[1].Trim();
                string s = parts[1];
                s = s.Replace("X", "x");
                s = s.Replace("d", "");
                s = s.Replace("D", "");
                bool isHex = s.IndexOf('x') >= 0;
                s = s.Replace("x", "");
                if (String.IsNullOrEmpty(s))
                {
                    byte number;
                    bool result = isHex? byte.TryParse(parts[0], NumberStyles.HexNumber, provider, out number) : 
                                                            byte.TryParse(parts[0], out number);
                    if (result)
                    {
                        return new byte[1] { number };
                    }
                    else
                        return null;
                }
                else
                {
                    byte numo;
                    bool result = byte.TryParse(s, out numo);
                    if (result)
                    {
                        var b = new byte[numo];
                        UInt64 number;
                        result = isHex ? UInt64.TryParse(parts[0], NumberStyles.HexNumber, provider, out number) :
                                                            UInt64.TryParse(parts[0], out number);
                        if (result)
                        {
                            for (uint i = 0; i < numo; i++)
                            {
                                b[i] = Convert.ToByte(number & 0xFF);
                                number = number >> 8;
                            }
                            return b;
                        }
                        else
                            return null;
                        
                    }
                    else
                        return null;
                }
            }
            else 
                return null;
        }
    }
}
