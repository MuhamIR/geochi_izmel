using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace MiniModbus
{

    public abstract partial class MiniModbusDevice
    {
        public MiniModbusDevice(byte id)
        {
            _gen_data = new ReadOnlyTable(MiniModbusCommand.READ_DEVICE_INFO_COMMAND, 6);
            _id = id;
            _request_to_write_ROM = false;
        }

        public void ChangeID(byte id)
        {
            _id = id;
        }

        public void SetWriteRomRequest()
        {
            _request_to_write_ROM = true;
        }

        public void StartCommunication()
        {
            _communication_thread = new Thread( new ThreadStart(this._processAllData));
            _need_to_stop = false;
            _communication_thread.Start();
        }

        public void StopCommunication()
        {
            _need_to_stop = true;
            _communication_thread.Join();
            _communication_thread = null;
        }

        public bool IsActive()
        {
            if (_communication_thread == null)
                return false;
            ThreadState state = _communication_thread.ThreadState;
            HashSet<ThreadState> active_states = new HashSet<ThreadState>{ ThreadState.Running, ThreadState.Background, ThreadState.WaitSleepJoin };
            return active_states.Contains(state);
        }

        public class ReadOnlyTable
        {
            public ReadOnlyTable(MiniModbusCommand cmd, int size)
            {
                ReadCmd = cmd;
                _data = new UInt16[size];
                for (uint i = 0; i < _data.Length; ++i)
                {
                    _data[i] = 0;
                }
                _dataWasUpdated = true;
            }



            public int Size
            {
                get { return _data.Length; }
            }

            public void _set(UInt16[] data)
            {
                for (uint i = 0; i < _data.Length; ++i)
                {
                    if (_data[i] != data[i])
                    {
                        _dataWasUpdated = true;
                        _data[i] = data[i];
                    }
                }
            }
            public UInt16[] ReadDeviceData()
            {
                return (UInt16[])_data.Clone();
            }

            public bool DataWasUpdated()
            {
                bool res = _dataWasUpdated;
                _dataWasUpdated = false;
                return res;
            }
            bool _dataWasUpdated;
            public readonly MiniModbusCommand ReadCmd;
            UInt16[] _data;
        }

        public class ReadWriteTable : ReadOnlyTable
        {
            public ReadWriteTable(MiniModbusCommand readcmd, MiniModbusCommand writecmd, int size)
                : base(readcmd, size)
            {
                WriteCmd = writecmd;
                _control_data = new UInt16[size];
                WriteFlag = true;
            }
            public void WriteDeviceData(UInt16[] data)
            {
                for (int i = 0; i < _control_data.Length; i++)
                {
                    _control_data[i] = data[i];
                }
            }

            public UInt16[] _readControl()
            {
                return (UInt16[])_control_data.Clone();
            }

            public bool WriteFlag { get; set; }
            public MiniModbusCommand WriteCmd;
            UInt16[] _control_data;
        }

        public ReadOnlyTable _gen_data;
        public ReadOnlyTable _RO_data;
        public ReadWriteTable _RW_data;
        public ReadWriteTable _RW_flash;

        public event EventHandler readingIsComplete;
        public event EventHandler cycleIsComplete;
        public event EventHandler deviceWasChanged;

        public uint GoodCyclesNumber { get; private set; }
        public uint AllCyclesNumber { get; private set; }
        public void ResetCounters(){GoodCyclesNumber = 0; AllCyclesNumber = 0;}
    }
}
