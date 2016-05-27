using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MiniModbus;
using PackedSerial;
using System.Threading;
using System.Diagnostics;

namespace SerialMiniMODBUS
{
    class SerialModbusDevice : MiniModbusDevice
    {
        const int max_read_size = 32;
        const int max_write_size = 32;
        public SerialModbusDevice(PackedSerialPort serial, byte id):base(id)
        {
            _serial = new PackedSerialPortShell(serial);
        }
        protected override bool readTable(ReadOnlyTable table, int timeoutval)
        {
	        Debug.Assert (table.Size != 0);

            UInt16[] receive_data = new UInt16[table.Size];

            int wasread = 0;

            while(wasread != table.Size)
            {
	            int size = table.Size - wasread;
	            if (size > max_read_size)
	            {
		            size = max_read_size;
	            }

                Debug.Assert (size < 0x80);
                Debug.Assert (wasread < 0x200);

	            byte[] tosend = new byte[4];
	            tosend[0] = _id;
                tosend[1] = (byte)table.ReadCmd;
                tosend[2] = (byte)wasread;
	            tosend[3] = (byte)size;
                if (wasread >= 0x100)
	            {
		            tosend[3] |= 0x80;
	            }

                if (!_serial.Send(tosend))
                    return false;

                byte[] toreceive = null;

                do
                {
                    Thread.Sleep(1); 
                    toreceive = _serial.Receive();                        
                }
                while ((toreceive == null) && (timeoutval-- != 0));

                if (toreceive == null)
                    return false;

                if (toreceive.Length != (4 + size * 2) )
                    return false;

                for (int i = 0; i < 4; i++)
                {
                    if (toreceive[i] != tosend[i])
                        return false;
                }

                for (int i = 0; i < size; i++)
                {
                    UInt16 t = Convert.ToUInt16(toreceive[4 + 2 * i] + (toreceive[4 + 2 * i + 1] << 8));
                    receive_data[wasread + i] = t;
                }

                wasread += size;
            }

            table._set(receive_data);
            return true;
        }
        protected override bool writeTable(ReadWriteTable table, int timeoutval)
        {
            Debug.Assert(table.Size != 0);

            if (table.WriteFlag)
            {
                UInt16[] transmit_data = table._readControl();

                int waswritten = 0;

                while (waswritten != table.Size)
                {
                    int size = table.Size - waswritten;
                    if (size > max_write_size)
                    {
                        size = max_write_size;
                    }

                    Debug.Assert(size < 0x80);
                    Debug.Assert(waswritten < 0x200);

                    byte[] tosend = new byte[4 + size*2];
                    tosend[0] = _id;
                    tosend[1] = (byte)table.WriteCmd;
                    tosend[2] = (byte)waswritten;
                    tosend[3] = (byte)size;
                    if (waswritten >= 0x100)
                    {
                        tosend[3] |= 0x80;
                    }

                    for (int i = 0; i < size; i++)
                    {
                        tosend[4 + 2*i] = Convert.ToByte(transmit_data[waswritten + i] & 0xff);
                        tosend[4 + 2*i + 1] = Convert.ToByte((transmit_data[waswritten + i] >> 8) & 0xff);
                    }

                    if (!_serial.Send(tosend))
                        return false;

                    byte[] toreceive = null;

                    do
                    {
                        Thread.Sleep(1);
                        toreceive = _serial.Receive();
                    }
                    while ((toreceive == null) && (timeoutval-- != 0));

                    if (toreceive == null)
                        return false;

                    if (toreceive.Length != 4)
                        return false;

                    
                    if (toreceive[0] != _id) return false;
                    if (toreceive[1] != (byte)MiniModbusCommand.SHORT_RESPONCE_COMMAND) return false;
                    if (toreceive[2] != (byte)table.WriteCmd) return false;
                    if (toreceive[3] != 0) return false;

                    waswritten += size;
                }
            }
            return true;
        }

        protected override bool writeRom(int timeoutval)
        {

            byte[] tosend = new byte[2];
            tosend[0] = _id;
            tosend[1] = (byte)MiniModbus.MiniModbusCommand.WRITE_FLASH_FROM_BUFFER_COMMAND;
            if (!_serial.Send(tosend))
                return false;

            byte[] toreceive = null;

            do
            {
                Thread.Sleep(1);
                toreceive = _serial.Receive();
            }
            while ((toreceive == null) && (timeoutval-- != 0));

            if (toreceive == null)
                return false;

            if (toreceive.Length != 4)
                return false;

            if (toreceive[0] != _id)
                return false;
            if (toreceive[1] != (byte)MiniModbus.MiniModbusCommand.SHORT_RESPONCE_COMMAND)
                return false;
            if (toreceive[2] != (byte)MiniModbus.MiniModbusCommand.WRITE_FLASH_FROM_BUFFER_COMMAND)
                return false;
            if (toreceive[3] != 0)
                return false;
            return true;
        }

        protected override void clearInput()
        {
            while (_serial.Receive() != null)
                ;
        }

        PackedSerialPortShell _serial;
    }
}
