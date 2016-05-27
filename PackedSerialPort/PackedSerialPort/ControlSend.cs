using System;
using System.Windows.Forms;

namespace PackedSerialApp
{
    public partial class ControlSend : UserControl
    {
        public ControlSend(FormMain main)
        {
            _main = main;
            _counter = 0;
            InitializeComponent();
            checkBoxShowSending.Checked = true;
            buttonSend.Click += new System.EventHandler(this.buttonSend_Click);
            
        }

        private void buttonSend_Click(object sender, EventArgs e)
        {
            _main.SendString(textBoxSend.Text, checkBoxShowSending.Checked);
        }

        public void timer10ms_Tick(object sender, EventArgs e)
        {
            if (checkBoxPeriodicSend.Checked)
            {
                uint limit = 0;
                if (uint.TryParse(textBoxPeriod.Text, out limit))
                {
                    _counter += 10;
                    if (_counter >= limit)
                    {
                        _counter = 0;
                        _main.SendString(textBoxSend.Text, checkBoxShowSending.Checked);
                    }
                }
                else
                {
                    _main.Println("Неверный формат, разрешение отправки отменено");
                    checkBoxPeriodicSend.Checked = false;
                }
            }
        }

        public void SerialPortStateIsChanged(object sender, PackedSerial.ChangeStateEventArgs e)
        {
            buttonSend.Enabled = e.IsOpen;
        }

        FormMain _main;
        uint _counter;
    }
}
