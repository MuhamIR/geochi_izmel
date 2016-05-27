using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using SerialMiniMODBUS;
using PackedSerial;
using MiniModbus;

namespace SerialMiniMODBUS
{
    public partial class FormMain : Form
    {
        public FormMain()
        {
            InitializeComponent();
            Utility.RefreshPortList(comboBoxAvailablePortList);
            if (comboBoxAvailablePortList.Items.Count > 0)
            {
                comboBoxAvailablePortList.SelectedItem = comboBoxAvailablePortList.Items[0];
            }
            _write_request = false;
        }

        SerialModbusDevice _serial_modbus;
        PackedSerialPort _serial_driver;
        MiniModbusDeviceTableShell _table_shell;

        private void buttonOpenClose_Click(object sender, EventArgs e)
        {
            if (_serial_modbus == null)
            {
                _serial_driver = new PackedSerialPort();
                _serial_driver.StateIsChanged += this.serialPortStateIsChanged;
                _serial_modbus = new SerialModbusDevice(_serial_driver, 1);
                _table_shell = new MiniModbusDeviceTableShell(_serial_modbus);
                _table_shell.TablesWereCreated += deviceWasChanged;
                _serial_modbus.readingIsComplete += readingIsComplete;
                
            }

            if (_serial_driver.IsOpen())
            {
                _serial_modbus.StopCommunication();
                _serial_driver.Close();
            }
            else
            {
                if (comboBoxAvailablePortList.SelectedItem != null)
                {
                    _serial_driver.Open(comboBoxAvailablePortList.SelectedItem.ToString(), 115200);
                }
            }           
        }

        bool _write_request;

        void readingIsComplete(object sender, EventArgs args)
        {
            if (_write_request)
            {
                _write_request = false;
                ushort[] rw = _serial_modbus._RW_data.ReadDeviceData();
                _serial_modbus._RW_data.WriteDeviceData(rw);
            }
            ushort[] ro = _serial_modbus._RO_data.ReadDeviceData();
            UInt32 repeat_num = (UInt32)ro[0] | (UInt32)ro[1]<<16;
            labelShowRepeatNum.Text = string.Format("Осталось: {0}", repeat_num);
        }



        private void deviceWasChanged(object sender, EventArgs e)
        {
            MiniModbusDeviceTableShell table_shell = (MiniModbusDeviceTableShell)sender;
            this.Invoke(new some_action(() =>
            {
                dataGridGeneralData.DataSource = table_shell.GeneralInfo.Table;
                dataGridROData.DataSource = table_shell.ROInfo.Table;
                dataGridRWData.DataSource = table_shell.RWInfo.Table;
                dataGridFlashData.DataSource = table_shell.RWFlashInfo.Table;
                DebugBox.Text = DebugBox.Text + "Обнаружено устройство - " + table_shell.Name + "\r\n";
            }));
        }

        public delegate void some_action();

        private void serialPortStateIsChanged(object sender, ChangeStateEventArgs e)
        {
            this.Invoke(new some_action(() =>
            {
                
                DebugBox.Text = DebugBox.Text + (e.IsOpen ? "Последовательный порт удачно открыт\r\n" :
                                                             "Последовательный порт удачно закрыт\r\n");
                
                buttonOpenClose.Text = e.IsOpen ? "Закрыть" : "Открыть";
                timer10ms.Enabled = e.IsOpen;
            }));
            
               
            if (e.IsOpen && !_serial_modbus.IsActive())
            {
                _serial_modbus.StartCommunication();
            }
        }

        private void comboBoxAvailablePortList_Click(object sender, EventArgs e)
        {
            Utility.RefreshPortList(comboBoxAvailablePortList);
        }

        private void timer10ms_Tick(object sender, EventArgs e)
        {
            labelCycleCounter.Text = string.Format("{0}/{1}", _serial_modbus.GoodCyclesNumber, _serial_modbus.AllCyclesNumber);
        }

        private void clearDebugBox(object sender, EventArgs e)
        {
            DebugBox.Text = "";
            _serial_modbus.ResetCounters();
        }

        private void button_Click_RomBufferToRom(object sender, EventArgs e)
        {
            _serial_modbus.SetWriteRomRequest();
        }

        private void checkBoxWriteRomBuffer_CheckedChanged(object sender, EventArgs e)
        {
            _serial_modbus._RW_flash.WriteFlag = checkBoxWriteRomBuffer.Checked;
        }
    }
}