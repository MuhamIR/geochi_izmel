using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace MiniModbus
{
    public enum MiniModbusCommand
    {
        READ_RO_COMMAND = 1,
        READ_DEVICE_INFO_COMMAND = 2,
        READ_RW_COMMAND = 3,
        WRITE_RW_COMMAND = 4,
        READ_FLASH_BUFFER_COMMAND = 5,
        WRITE_FLASH_BUFFER_COMMAND = 6,
        WRITE_FLASH_FROM_BUFFER_COMMAND = 7,
        SHORT_RESPONCE_COMMAND = 65
    }
    public abstract partial class MiniModbusDevice
    {
        void _processAllData()
        {
            while (_need_to_stop == false)
            {
                int interdelay = 1;
                Thread.Sleep(20); 
                clearInput();
                AllCyclesNumber++;
                if (readTable(_gen_data, 100))
                {
                    if (_gen_data.DataWasUpdated())
                    {
                        UInt16[] t = _gen_data.ReadDeviceData();
                        _RO_data = new ReadOnlyTable(MiniModbusCommand.READ_RO_COMMAND, t[3]);
                        _RW_data = new ReadWriteTable(MiniModbusCommand.READ_RW_COMMAND, MiniModbusCommand.WRITE_RW_COMMAND, t[4]);
                        _RW_flash = new ReadWriteTable(MiniModbusCommand.READ_FLASH_BUFFER_COMMAND, MiniModbusCommand.WRITE_FLASH_BUFFER_COMMAND, t[5]);
                        _RW_flash.WriteFlag = false;
                        if (deviceWasChanged != null)
                        {
                            deviceWasChanged(this, null);
                            ResetCounters();
                        }
                    }
                }
                else
                    continue;

                Thread.Sleep(interdelay);
                if (readTable(_RO_data, 100) == false)
                    continue;
                Thread.Sleep(interdelay);
                if (readTable(_RW_data, 100) == false)
                    continue;
                Thread.Sleep(interdelay);
                if (readTable(_RW_flash, 100) == false)
                    continue;
                Thread.Sleep(interdelay);
                if (readingIsComplete != null)
                {
                    readingIsComplete(this, null);
                }
                Thread.Sleep(interdelay);
                if (writeTable(_RW_data, 100) == false)
                    continue;
                Thread.Sleep(interdelay);
                if (writeTable(_RW_flash, 100) == false)
                    continue;
                if (_request_to_write_ROM)
                {
                    _request_to_write_ROM = false;
                    if (writeRom(100) == false)
                        continue;
                }

                GoodCyclesNumber++;
                if (cycleIsComplete != null)
                {
                    cycleIsComplete(this, null);
                }
            }
        }


        protected abstract bool readTable(ReadOnlyTable value, int timeoutval);
        protected abstract bool writeTable(ReadWriteTable value, int timeoutval);
        protected abstract bool writeRom(int timeoutval);
        protected abstract void clearInput();

        Thread _communication_thread;
        bool _need_to_stop;
        bool _request_to_write_ROM;
        protected byte _id;

    }
}
