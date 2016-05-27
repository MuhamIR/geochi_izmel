namespace PackedSerialApp
{
    partial class ControlSend
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.groupBoxSend = new System.Windows.Forms.GroupBox();
            this.checkBoxShowSending = new System.Windows.Forms.CheckBox();
            this.checkBoxPeriodicSend = new System.Windows.Forms.CheckBox();
            this.textBoxPeriod = new System.Windows.Forms.TextBox();
            this.textBoxSend = new System.Windows.Forms.TextBox();
            this.buttonSend = new System.Windows.Forms.Button();
            this.groupBoxSend.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBoxSend
            // 
            this.groupBoxSend.Controls.Add(this.checkBoxShowSending);
            this.groupBoxSend.Controls.Add(this.checkBoxPeriodicSend);
            this.groupBoxSend.Controls.Add(this.textBoxPeriod);
            this.groupBoxSend.Controls.Add(this.textBoxSend);
            this.groupBoxSend.Controls.Add(this.buttonSend);
            this.groupBoxSend.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBoxSend.Location = new System.Drawing.Point(0, 0);
            this.groupBoxSend.Name = "groupBoxSend";
            this.groupBoxSend.Size = new System.Drawing.Size(266, 93);
            this.groupBoxSend.TabIndex = 10;
            this.groupBoxSend.TabStop = false;
            this.groupBoxSend.Text = "Отправка";
            // 
            // checkBoxShowSending
            // 
            this.checkBoxShowSending.AutoSize = true;
            this.checkBoxShowSending.Location = new System.Drawing.Point(64, 71);
            this.checkBoxShowSending.Name = "checkBoxShowSending";
            this.checkBoxShowSending.Size = new System.Drawing.Size(89, 17);
            this.checkBoxShowSending.TabIndex = 5;
            this.checkBoxShowSending.Text = "Показывать";
            this.checkBoxShowSending.UseVisualStyleBackColor = true;
            // 
            // checkBoxPeriodicSend
            // 
            this.checkBoxPeriodicSend.AutoSize = true;
            this.checkBoxPeriodicSend.Location = new System.Drawing.Point(64, 48);
            this.checkBoxPeriodicSend.Name = "checkBoxPeriodicSend";
            this.checkBoxPeriodicSend.Size = new System.Drawing.Size(87, 17);
            this.checkBoxPeriodicSend.TabIndex = 4;
            this.checkBoxPeriodicSend.Text = "Периодично";
            this.checkBoxPeriodicSend.UseVisualStyleBackColor = true;
            // 
            // textBoxPeriod
            // 
            this.textBoxPeriod.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F);
            this.textBoxPeriod.Location = new System.Drawing.Point(6, 55);
            this.textBoxPeriod.Name = "textBoxPeriod";
            this.textBoxPeriod.Size = new System.Drawing.Size(40, 23);
            this.textBoxPeriod.TabIndex = 3;
            this.textBoxPeriod.Text = "1000";
            // 
            // textBoxSend
            // 
            this.textBoxSend.Font = new System.Drawing.Font("Times New Roman", 11F);
            this.textBoxSend.Location = new System.Drawing.Point(6, 19);
            this.textBoxSend.Name = "textBoxSend";
            this.textBoxSend.Size = new System.Drawing.Size(254, 24);
            this.textBoxSend.TabIndex = 2;
            // 
            // buttonSend
            // 
            this.buttonSend.Enabled = false;
            this.buttonSend.Font = new System.Drawing.Font("Times New Roman", 12F);
            this.buttonSend.Location = new System.Drawing.Point(157, 48);
            this.buttonSend.Name = "buttonSend";
            this.buttonSend.Size = new System.Drawing.Size(103, 40);
            this.buttonSend.TabIndex = 0;
            this.buttonSend.Text = "Отправить";
            this.buttonSend.UseVisualStyleBackColor = true;
            // 
            // ControlSend
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBoxSend);
            this.Name = "ControlSend";
            this.Size = new System.Drawing.Size(266, 93);
            this.groupBoxSend.ResumeLayout(false);
            this.groupBoxSend.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBoxSend;
        private System.Windows.Forms.CheckBox checkBoxShowSending;
        private System.Windows.Forms.CheckBox checkBoxPeriodicSend;
        private System.Windows.Forms.TextBox textBoxPeriod;
        private System.Windows.Forms.TextBox textBoxSend;
        private System.Windows.Forms.Button buttonSend;
    }
}
