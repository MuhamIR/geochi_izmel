using System;
using System.Collections.Generic;
using System.Collections.Concurrent;
using System.IO.Ports;

namespace PackedSerial
{
    ///<summary>
    ///ActionResult служит для выдачи результата в нетривиальных функциях работы с последовательным портом, 
    ///Item1 - результат операции
    ///Item2 - описание результата
    /// </summary>
    using ActionResult = System.Tuple<bool, string>;

    public class PacketReceivedEventArgs : EventArgs 
    {
        public PacketReceivedEventArgs(byte[] recpacket)
        {
            _packet = recpacket;
        }
        public byte[] GetPacket() 
        { 
            var b = new byte[_packet.Length];
            _packet.CopyTo(b, 0);
            return b; 
        }

        byte[] _packet; 
    }


    public class ChangeStateEventArgs : EventArgs
    {
        public ChangeStateEventArgs(bool nowIsOpen)
        {
            _nowIsOpen = nowIsOpen;
        }
        public bool IsOpen { get { return _nowIsOpen; } }

        bool _nowIsOpen;
        
    }
    
    /// <summary>
    /// Класс PackedSerialPort реализует приемо-перадачу пакетов по последовательному порту по протоколу, принятому в Технопрактике
    /// </summary>
    public class PackedSerialPort
    {
        

        public PackedSerialPort()
        {
            _serial = new SerialPort();
            _serial.DataReceived += this._receivedDataHandler;
            _recBuf = new byte[_bufSize];
            _startFlag = false;
            _counter = 0;
        }


        /// <summary>
        /// Для получения пакета необходимо подписаться на событие PacketReceived, 
        /// необходимо помнить, что переданный делегат будит выполнен в каком-то системном потоке
        /// </summary>
        public event EventHandler<PacketReceivedEventArgs> PacketReceived;

        /// <summary>
        /// Событие открытия - закрытия последовательного порта
        /// </summary>
        public event EventHandler<ChangeStateEventArgs> StateIsChanged;
        

        public bool IsOpen()
        {
            return _serial.IsOpen;
        }

        /// <summary>
        /// Метод Open предоставляет возможность полной настройки последовательного порта, при этом настройки по умолчанию соответствуют
        /// стандартным в Технопрактике 
        /// </summary>
        /// <param name="portName">номер порта в виде строки( например, "COM1")</param>
        /// <param name="baudRate"></param>
        /// <param name="parity"></param>
        /// <param name="dataBits"></param>
        /// <param name="stopBits"></param>
        /// <param name="handshake"></param>
        /// <returns></returns>
        public ActionResult Open(string portName, int baudRate = 115200, Parity parity = Parity.None,
                                        int dataBits = 8, StopBits stopBits = StopBits.One, Handshake handshake = Handshake.None)
        {
            try
            {
                _serial.PortName = portName;
                _serial.BaudRate = baudRate;
                _serial.Parity = parity;
                _serial.DataBits = dataBits;
                _serial.StopBits = stopBits;
                _serial.Handshake = handshake;
                _serial.Open();
                if (StateIsChanged != null)
                {
                    StateIsChanged(this, new ChangeStateEventArgs(true));
                }
            }
            catch (Exception e)
            {
                return Tuple.Create(false, e.Message);
            }
            return Tuple.Create(true, "Последовательный порт удачно открыт");
        }

        public ActionResult Close()
        {
            try
            {
                _serial.Close();
                if (StateIsChanged != null)
                {
                    StateIsChanged(this, new ChangeStateEventArgs(false));
                }
            }
            catch (Exception e)
            {
                return Tuple.Create(false, e.Message);
            }
            return Tuple.Create(true, "Последовательный порт удачно закрыт");
        }

        /// <summary>
        /// Функция отправки пакета данных
        /// </summary>
        /// <param name="decapsulated">данные для отправки</param>
        /// <returns></returns>
        public ActionResult Send(byte[] value)
        {
            try
            {
                byte[] tosend = _convertToPacket(value);
                _serial.Write(tosend, 0, tosend.Length);
            }
            catch (Exception e)
            {
                return Tuple.Create(false, e.Message);
            }
            return Tuple.Create(true, "Удачная отправка");
        }


        #region _internal
        UInt16 _calcCRC16 (UInt16 dataCRC16, byte  data)  // Aaeo
        {
          byte index = Convert.ToByte(((dataCRC16 >> 8) ^ (uint)data) & (uint)0x00FF);
          return Convert.ToUInt16((tableCRC16[index] ^ (dataCRC16 << 8)) & 0x0000ffff);
        }

        UInt16 _crc16(byte[] pcBlock, int size = -1)
        {
            UInt16 crc = 0;
            if (size <= 0)
            {
                size = pcBlock.Length;
            }
            for (uint i = 0; i < size; ++i)
            {
                crc = _calcCRC16(crc, pcBlock[i]);
            }
	        return crc;
        }

        byte[] _convertToPacket(byte[] decapsulated)
        {
            List<byte> list = new List<byte>();
            UInt16 crc = _crc16(decapsulated);
            list.Add((byte)SpecSimbols.Start);
            foreach (byte b in decapsulated)
            {
                _add(list, b);
            }
            _add(list, Convert.ToByte(crc & 0xFF));
            _add(list, Convert.ToByte(crc >> 8));
            list.Add((byte)SpecSimbols.End);
            return list.ToArray();
        }

        static void _add(List<byte> list, byte b)
        {
            switch (b)
            {
                case (byte)SpecSimbols.Start:
                    list.Add((byte)SpecSimbols.Escape);
                    list.Add((byte)SpecSimbols.AltStart);
                    break;
                case (byte)SpecSimbols.End:
                    list.Add((byte)SpecSimbols.Escape);
                    list.Add((byte)SpecSimbols.AltEnd);
                    break;
                case (byte)SpecSimbols.Escape:
                    list.Add((byte)SpecSimbols.Escape);
                    list.Add((byte)SpecSimbols.AltEscape);
                    break;
                default:
                    list.Add(b);
                    break;
            }
        }


        void _receivedDataHandler(Object sender, SerialDataReceivedEventArgs e)
        {
            int readn;
            while(true)
            {
                if (_serial.BytesToRead == 0)
                {
                    return;
                }
                readn = _serial.ReadByte();
                if (readn == -1)
                {
                    return;
                }
                else
                {
                    byte b = Convert.ToByte(readn);

                    if (b == (byte)SpecSimbols.Start)//если были в состоянии старт нужно генерировать ошибку
                    {
                        _resetReceiving();
                        _startFlag = true;
                    }
                    else if (_startFlag)
                    {
                        if (b == (byte)SpecSimbols.End)
                        {
                            _processPacket();
                            _resetReceiving();
                        }
                        else
                        {
                            if (_counter < _bufSize)
                            {
                                _recBuf[_counter++] = b;
                            }
                            else
                            {
                                _resetReceiving();//генерировать ошибку
                            }
                        }
                    }
                }
            }
           
        }

        void _resetReceiving()
        {
            _startFlag = false;
            _counter = 0;
        }

        void _processPacket()
        {
            if(_counter >= 3)
            {
                List<byte> list = new List<byte>();
                for (uint i = 0; i < _counter; i++)
                {
                    switch(_recBuf[i])
                    {
                        case (byte)SpecSimbols.Escape:
                            if( i < (_recBuf.Length-1))
                            {
                                i++;
                                switch(_recBuf[i])
                                {
                                    case (byte)SpecSimbols.AltEscape:
                                        list.Add((byte)SpecSimbols.Escape);
                                        break;
                                    case (byte)SpecSimbols.AltStart:
                                        list.Add((byte)SpecSimbols.Start);
                                        break;
                                    case (byte)SpecSimbols.AltEnd:
                                        list.Add((byte)SpecSimbols.End);
                                        break;
                                    default:
                                        return;
                                }
                            }
                            else
                            {
                                return;
                                //ошибка
                            }
                            break;
                        default:
                            list.Add(_recBuf[i]);
                            break;
                    }
                }
                UInt16 crc = Convert.ToUInt16((list[list.Count - 1] << 8) | list[list.Count - 2]);
                list.RemoveRange(list.Count - 2, 2);
                byte[] t = list.ToArray();
                
                if (crc == _crc16(t))
                {
                    if (PacketReceived != null)
                    {
                        PacketReceived(this, new PacketReceivedEventArgs(t));
                    }
                }
            }
            else
            {
                //генерировать ошибку
            }
        }

        enum SpecSimbols : byte { Start = 0x24, End = 0xfe, Escape = 0x1b, AltEscape = 0x01, AltStart = 2, AltEnd = 3 };
        readonly UInt16[] tableCRC16 = new UInt16[]{  //may be U16
                  0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
                  0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
                  0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
                  0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
                  0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
                  0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
                  0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
                  0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
                  0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
                  0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
                  0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
                  0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
                  0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
                  0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
                  0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
                  0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
                  0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
                  0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
                  0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
                  0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
                  0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
                  0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
                  0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
                  0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
                  0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
                  0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
                  0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
                  0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
                  0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
                  0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
                  0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
                  0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
                };

        SerialPort _serial;
        byte[] _recBuf;
        bool _startFlag;
        uint _counter;
        const uint _bufSize = 512;
        #endregion
    }

    /// <summary>
    /// Сборник статических функций для упрощения работы с последовательным портом
    /// </summary>
    static public class Utility
    {
        /// <summary>
        /// Устанавливает в переданном ComboBox список доступных портов
        /// </summary>
        /// <param name="combobox"></param>
        public static void RefreshPortList(System.Windows.Forms.ComboBox combobox)
        {
            combobox.Items.Clear();
            string[] ports = System.IO.Ports.SerialPort.GetPortNames();
            for (int i = ports.Length - 1; i >= 0; i--)
            {
                combobox.Items.Add(ports[i]);
            }
        }

        /// <summary>
        /// Устанавливает в переданном combobox список стандартных скоростей
        /// </summary>
        /// <param name="combobox"></param>
        public static void SetStandartSpeeds(System.Windows.Forms.ComboBox combobox)
        {
            combobox.Items.Clear();
            string[] speeds = new string[] { "9600", "19200", "38400", "57600", "115200", "230400" };
            foreach (string s in speeds)
            {
                combobox.Items.Add(s);
            }
        }
        /// <summary>
        /// Преобразовывает скорость заданную в виде строки, в int. Если скорость не входит в список 
        /// стандартных скоростей возвращается -1
        /// </summary>
        public static int ConvertSpeed(string speed)
        {
            int converted_speed = Convert.ToInt32(speed);
            int[] standart_speeds = new int[] { 115200, 9600, 19200, 38400, 57600, 230400 };
            return (System.Array.IndexOf(standart_speeds, converted_speed) >= 0) ? converted_speed : -1;
        }
    }

    public class PackedSerialPortShell
    {
        public PackedSerialPortShell(PackedSerialPort serial)
        {
            _serial = serial;
            _serial.PacketReceived += this._receive;
            _received_packets = new ConcurrentQueue<byte[]>();
        }

        public bool Send(byte[] value)
        {
            return _serial.Send(value).Item1;
        }

        public byte[] Receive()
        {
            byte[] p;
            if (_received_packets.TryDequeue(out p))
            {
                return p;
            }
            else
            {
                return null;
            }
        }

        void _receive(object sender, PacketReceivedEventArgs e)
        {
            _received_packets.Enqueue(e.GetPacket());
        }

        ConcurrentQueue<byte[]> _received_packets;
        PackedSerialPort _serial;
    }
}
