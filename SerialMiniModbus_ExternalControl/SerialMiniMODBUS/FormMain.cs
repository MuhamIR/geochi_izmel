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
using Newtonsoft.Json;
using System.IO;

namespace SerialMiniMODBUS
{
    public partial class FormMain : Form
    {
        public readonly int CountOfOutputs = 8;
        


        String configuration_filename = "config.json.txt";
        board_settings_t board_settings = new board_settings_t();

        public static FormMain form;

        public FormMain()
        {
            InitializeComponent();

            try
            {   // Open the text file using a stream reader.
                using (StreamReader sr = new StreamReader(configuration_filename))
                {
                    // Read the stream to a string, and write the string to the console.
                    String json = sr.ReadToEnd();
                    board_settings = JsonConvert.DeserializeObject<board_settings_t>(json);
                    println("Configuration was read from configuration file");
                }
            }
            catch (Exception e)
            {
                println("There is no configuration file");
                
                using (StreamWriter sw = new StreamWriter(configuration_filename))
                {
                    string json = JsonConvert.SerializeObject(board_settings, Formatting.Indented);
                    sw.Write(json);
                }
                println("Configuration file was created with default settings");
            }
            form = this;
            start_serial_communication();
        }

        SerialModbusDevice _serial_modbus;
        PackedSerialPort _serial_driver;
        MiniModbusDeviceTableShell _table_shell;

        private void start_serial_communication()
        {
            _serial_driver = new PackedSerialPort();
            _serial_driver.StateIsChanged += this.serialPortStateIsChanged;
            _serial_modbus = new SerialModbusDevice(_serial_driver, (byte)board_settings.__bus_id);

            _table_shell = new MiniModbusDeviceTableShell_ExternalControl(_serial_modbus, board_settings);
            _table_shell.TablesWereCreated += deviceWasChanged;

            _serial_driver.Open(board_settings.serial_port, 115200);
        }

        private void println(String line)
        {
            DebugBox.Text = DebugBox.Text + line + "\r\n";
        }

        public static void ext_println(string line)
        {
            FormMain.form.Invoke(new some_action(() =>
            {
                FormMain.form.DebugBox.Text = form.DebugBox.Text + line + "\r\n";
            }));
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
                DebugBox.Text = DebugBox.Text + "Device was detected - " + table_shell.Name + "\r\n";
            }));
        }

        public delegate void some_action();

        private void serialPortStateIsChanged(object sender, ChangeStateEventArgs e)
        {                
            DebugBox.Text = DebugBox.Text + (e.IsOpen ? "Serial port was opened\r\n" :
                                                            "Serial port was closed\r\n");
            timer10ms.Enabled = e.IsOpen;              
            if (e.IsOpen && !_serial_modbus.IsActive())
            {
                _serial_modbus.StartCommunication();
            }
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


    }

    public class output_settings_t
    {
        public output_settings_t(int start, int duration)
        {
            start_time_ms = start;
            duration_ms = duration;
        }
        public int start_time_ms;
        public int duration_ms;
    }
    public class board_settings_t
    {
        public string input_filename = "input.txt";
        public string output_filename = "output.txt";
        public string serial_port = "COM1";
        public int period_ms = 2500;
        public output_settings_t[] output_settings = new output_settings_t[]
        {   new output_settings_t(0, 500), new output_settings_t(400, 500), new output_settings_t(800, 500), new output_settings_t(1200, 500),
                new output_settings_t(1600, 500), new output_settings_t(0, 0), new output_settings_t(0, 0), new output_settings_t(0, 0)
        };
        public int __bus_id = 1;
    }

    public class MiniModbusDeviceTableShell_ExternalControl : MiniModbusDeviceTableShell
    {
        board_settings_t settings;
        public MiniModbusDeviceTableShell_ExternalControl(MiniModbusDevice device, board_settings_t settings) : base(device)
        {
            _device = device;
            _device.readingIsComplete += this.readingIsComplete;
            _device.deviceWasChanged += this.deviceWasChanged;
            this.settings = settings;
        }

        void set_rw(int toogle, int pause, int repetitions)
        {
            RWInfo.Table.Rows[0]["Запись"] = toogle.ToString();
            RWInfo.Table.Rows[1]["Запись"] = pause.ToString();
            RWInfo.Table.Rows[2]["Запись"] = repetitions.ToString();
        }

        void set_flash_rw()
        {
            RWFlashInfo.Table.Rows[0]["Запись"] = settings.__bus_id.ToString();
            RWFlashInfo.Table.Rows[1]["Запись"] = "1";
            RWFlashInfo.Table.Rows[2]["Запись"] = settings.period_ms.ToString();
            RWFlashInfo.Table.Rows[3]["Запись"] = "0";
            for (int i = 0; i < 8; i++)
            {
                RWFlashInfo.Table.Rows[4 + 2 * i + 0]["Запись"] = settings.output_settings[i].start_time_ms;
                RWFlashInfo.Table.Rows[4 + 2 * i + 1]["Запись"] = settings.output_settings[i].duration_ms;
            }
        }

        int get_current_toggle()
        {
            return Convert.ToInt32(RWInfo.Table.Rows[0]["Чтение"].ToString());
        }

        void start_command(int pause, int repetitions)
        {
            set_rw((get_current_toggle() == 1) ? 0 : 1, pause, repetitions);
        }

        void start_crushing(int count)
        {
            start_command(0, count);
        }

        void deviceWasChanged(object sender, EventArgs args)
        {
            UInt16[] gendata = _device._gen_data.ReadDeviceData();
            if (gendata[0] != 0x1205)
            {
                throw new Exception("Found device is not crusher");
            }
            _device._RW_flash.WriteFlag = true;
            set_flash_rw();

            //RWFlashInfo.Table.Rows[0]["Запись"] = "1";
            //set_rw(0, 0, 0);
        }

        string command;
        int count;
        enum Step { Initialization, Initialization2, WaitingForCommand, WaitingForCommandConfirm,  WaitingForEndOfCommand };
        Step step = Step.Initialization;
        void readingIsComplete(object sender, EventArgs args)
        {
            switch (step)
            {
                case Step.Initialization:
                    step = Step.Initialization2;
                    start_crushing(0);
                    break;
                case Step.Initialization2:
                    {
                        bool done = true;
                        for (int i = 0; i < 3; i++)
                        {
                            if (Convert.ToInt32(RWInfo.Table.Rows[i]["Запись"]) != Convert.ToInt32(RWInfo.Table.Rows[i]["Чтение"]))
                            {
                                done = false;
                            }
                        }
                        for (int i = 0; i < 20; i++)
                        {
                            if (Convert.ToInt32(RWFlashInfo.Table.Rows[i]["Запись"]) != Convert.ToInt32(RWFlashInfo.Table.Rows[i]["Чтение"]))
                            {
                                done = false;
                            }
                        }
                        if (done)
                        {
                            step = Step.WaitingForCommand;
                            _device._RW_flash.WriteFlag = false;

                            using (StreamWriter sw = new StreamWriter(settings.input_filename))
                            {
                                sw.Write("");
                            }
                            using (StreamWriter sw = new StreamWriter(settings.output_filename))
                            {
                                sw.Write("");
                            }
                        }
                    }
                    break;
                case Step.WaitingForCommand:
                    command = null;
                    using (StreamReader sr = new StreamReader(settings.input_filename))
                    {
                        string input = sr.ReadToEnd();
                        if (input != "")
                        {
                            var words = input.Split(' ');
                            if ((words.Length == 1) && (words[0] == "nop"))
                            {
                                command = words[0];
                            }
                            else if ((words.Length == 2) && (words[0] == "start"))
                            {
                                if (Int32.TryParse(words[1], out count))
                                {
                                    command = words[0];                                    
                                }
                                else
                                {
                                    FormMain.ext_println("cannot parse " + words[1] + " as integer");
                                }
                            }
                        }                        
                    }
                    if (command != null)
                    {
                        using (StreamWriter sw = new StreamWriter(settings.input_filename))
                        {
                            sw.Write("");
                        }
                        step = Step.WaitingForCommandConfirm;
                        FormMain.ext_println("Command \"" + command + "\" was received");
                        if (command == "nop")
                        {
                            start_crushing(0);
                        }
                        else if (command == "start")
                        {
                            start_crushing(count);
                        }
                    }
                    break;
                case Step.WaitingForCommandConfirm:
                    {
                        bool done = true;
                        for (int i = 0; i < 3; i++)
                        {
                            if (Convert.ToInt32(RWInfo.Table.Rows[i]["Запись"]) != Convert.ToInt32(RWInfo.Table.Rows[i]["Чтение"]))
                            {
                                done = false;
                            }
                        }
                        if (done)
                        {
                            step = Step.WaitingForEndOfCommand;                          
                        }
                    }
                    break;
                case Step.WaitingForEndOfCommand:
                    {
                        if (ROInfo.Table.Rows[0]["Чтение"].ToString() == "0")
                        {
                            FormMain.ext_println("Command \"" + command + "\" was completed");
                            using (StreamWriter sw = new StreamWriter(settings.output_filename))
                            {
                                sw.Write(command + " 0");
                            }
                            step = Step.WaitingForCommand;
                        }
                        
                    }
                    break;
            }
                    
        }
    }

}