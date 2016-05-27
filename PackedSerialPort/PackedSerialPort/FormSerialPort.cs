using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using PackedSerial;

[assembly: CLSCompliant(true)]
namespace PackedSerialApp
{
    public partial class FormMain : Form
    {
        PackedSerialPort _port;
        public delegate void action();
        
        public FormMain()
        {
            InitializeComponent();
            _port = new PackedSerialPort();
            _port.StateIsChanged += this.serialPortStateIsChanged;
            _port.PacketReceived += this.PrintReceived;
            
            Utility.RefreshPortList(comboBoxAvailablePortList);
            if (comboBoxAvailablePortList.Items.Count > 0)
            {
                comboBoxAvailablePortList.SelectedItem = comboBoxAvailablePortList.Items[0];
            }
            Utility.SetStandartSpeeds(comboBoxAvailableSpeeds);
            comboBoxAvailableSpeeds.SelectedItem = comboBoxAvailableSpeeds.Items[4];
            

            _sender_list = new List<ControlSend>();

            
            addSenderPanel();

            serialPortStateIsChanged(this, new ChangeStateEventArgs(false));
        }

        void addSenderPanel()
        {
            ControlSend send_panel = new ControlSend(this);
            flowLayoutPanelLeft.Controls.Add(send_panel);
            timer10ms.Tick += send_panel.timer10ms_Tick;
            _port.StateIsChanged += send_panel.SerialPortStateIsChanged;
            _sender_list.Add(send_panel);
        }


        private void serialPortStateIsChanged(object sender, ChangeStateEventArgs e)
        {
            buttonOpenClose.Text = e.IsOpen ? "Закрыть" : "Открыть";
            comboBoxAvailableSpeeds.Enabled = !e.IsOpen;
            comboBoxAvailablePortList.Enabled = !e.IsOpen;
            timer10ms.Enabled = e.IsOpen;
        }

        private void buttonOpenClose_Click(object sender, EventArgs e)
        {
            if (_port.IsOpen())
            {
                _port.Close();
            }
            else
            {
                if (comboBoxAvailablePortList.SelectedItem != null)
                {
                    int speed = Utility.ConvertSpeed(comboBoxAvailableSpeeds.SelectedItem.ToString());
                    if (speed >= 0)
                    {
                        _port.Open(comboBoxAvailablePortList.SelectedItem.ToString(), speed);
                    }
                }
            }
        }

        void printTransmit(byte[] bytes)
        {
            Encoding encode = Encoding.GetEncoding(1251);
            if (checkBoxHEXFormat.Checked)
            {
                string s = "";
                foreach (byte b in bytes)
                {
                    s += string.Format("{0:X2} ", b);
                }
                Println(">>" + s);
            }
            else
            {
                Println(">>" + encode.GetString(bytes));
            }
        }

        public void SendString(string toconvert, bool printing = true)
        {
            if (_port.IsOpen())
            {
                byte[] buf = AddUtility.AddUtils.ConvertToSend(toconvert);
                if (buf != null)
                {
                    if (printing)
                    {
                        printTransmit(buf);
                    }
                    _port.Send(buf);
                }
                else
                {
                    Println("Неверный формат");
                }
            }
        }

        void PrintReceived(object sender, PacketReceivedEventArgs e)
        {
            textBoxPrint.Invoke(new action(() => 
            {
                byte[] bb = e.GetPacket();
                Encoding encode = Encoding.GetEncoding(1251);
                if (checkBoxHEXFormat.Checked)
                {
                    string s = "";
                    
                    foreach (byte b in bb)
                    {
                        s += string.Format("{0:X2} ", b);
                    }
                    Println("<<" + s);
                }
                else
                {
                    Println("<<" + encode.GetString(bb));
                }

            }
            ) );
        }


        private void comboBoxAvailablePortList_Click(object sender, EventArgs e)
        {
            Utility.RefreshPortList(comboBoxAvailablePortList);
        }

        public void Println(string toprint)
        {
            textBoxPrint.AppendText(toprint);
            textBoxPrint.AppendText("\n");
        }

        private void buttonClearPrint_Click(object sender, EventArgs e)
        {
            textBoxPrint.Text = "";
        }


        List<ControlSend> _sender_list;

        private void buttonAddSendPanel_Click(object sender, EventArgs e)
        {
            addSenderPanel();
        }
    }
}
