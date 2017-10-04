namespace SerialMiniMODBUS
{
    partial class FormMain
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
            if ((_serial_driver != null) && _serial_driver.IsOpen())
            {
                _serial_modbus.StopCommunication();
                _serial_driver.Close();
            }
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle1 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle2 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle3 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle4 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle5 = new System.Windows.Forms.DataGridViewCellStyle();
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.dataGridROData = new System.Windows.Forms.DataGridView();
            this.dataGridRWData = new System.Windows.Forms.DataGridView();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.tableLayoutPanel4 = new System.Windows.Forms.TableLayoutPanel();
            this.dataGridFlashData = new System.Windows.Forms.DataGridView();
            this.panel3 = new System.Windows.Forms.Panel();
            this.checkBoxWriteRomBuffer = new System.Windows.Forms.CheckBox();
            this.button3 = new System.Windows.Forms.Button();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.dataGridGeneralData = new System.Windows.Forms.DataGridView();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.DebugBox = new System.Windows.Forms.TextBox();
            this.panel2 = new System.Windows.Forms.Panel();
            this.textBoxNetId = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.labelCycleCounter = new System.Windows.Forms.Label();
            this.button2 = new System.Windows.Forms.Button();
            this.comboBoxAvailableSpeeds = new System.Windows.Forms.ComboBox();
            this.comboBoxAvailablePortList = new System.Windows.Forms.ComboBox();
            this.buttonOpenClose = new System.Windows.Forms.Button();
            this.timer10ms = new System.Windows.Forms.Timer(this.components);
            this.tableLayoutPanel2.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridROData)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridRWData)).BeginInit();
            this.tabPage2.SuspendLayout();
            this.tableLayoutPanel4.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridFlashData)).BeginInit();
            this.panel3.SuspendLayout();
            this.tabPage3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridGeneralData)).BeginInit();
            this.tableLayoutPanel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // tableLayoutPanel2
            // 
            this.tableLayoutPanel2.ColumnCount = 2;
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 194F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.Controls.Add(this.tabControl1, 1, 0);
            this.tableLayoutPanel2.Controls.Add(this.tableLayoutPanel1, 0, 0);
            this.tableLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel2.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 1;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.Size = new System.Drawing.Size(845, 473);
            this.tableLayoutPanel2.TabIndex = 65;
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Controls.Add(this.tabPage3);
            this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl1.Location = new System.Drawing.Point(197, 3);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(645, 467);
            this.tabControl1.TabIndex = 64;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.splitContainer1);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(637, 441);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "RO+RW";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(3, 3);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.dataGridROData);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.dataGridRWData);
            this.splitContainer1.Size = new System.Drawing.Size(631, 435);
            this.splitContainer1.SplitterDistance = 308;
            this.splitContainer1.TabIndex = 2;
            // 
            // dataGridROData
            // 
            this.dataGridROData.AllowUserToAddRows = false;
            this.dataGridROData.AllowUserToDeleteRows = false;
            this.dataGridROData.AllowUserToResizeRows = false;
            this.dataGridROData.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            dataGridViewCellStyle1.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle1.BackColor = System.Drawing.SystemColors.Window;
            dataGridViewCellStyle1.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            dataGridViewCellStyle1.ForeColor = System.Drawing.SystemColors.ControlText;
            dataGridViewCellStyle1.SelectionBackColor = System.Drawing.SystemColors.Highlight;
            dataGridViewCellStyle1.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle1.WrapMode = System.Windows.Forms.DataGridViewTriState.False;
            this.dataGridROData.DefaultCellStyle = dataGridViewCellStyle1;
            this.dataGridROData.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dataGridROData.Location = new System.Drawing.Point(0, 0);
            this.dataGridROData.Name = "dataGridROData";
            this.dataGridROData.Size = new System.Drawing.Size(308, 435);
            this.dataGridROData.TabIndex = 57;
            // 
            // dataGridRWData
            // 
            this.dataGridRWData.AllowUserToAddRows = false;
            this.dataGridRWData.AllowUserToDeleteRows = false;
            this.dataGridRWData.AllowUserToResizeRows = false;
            this.dataGridRWData.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            dataGridViewCellStyle2.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle2.BackColor = System.Drawing.SystemColors.Window;
            dataGridViewCellStyle2.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            dataGridViewCellStyle2.ForeColor = System.Drawing.SystemColors.ControlText;
            dataGridViewCellStyle2.SelectionBackColor = System.Drawing.SystemColors.Highlight;
            dataGridViewCellStyle2.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle2.WrapMode = System.Windows.Forms.DataGridViewTriState.False;
            this.dataGridRWData.DefaultCellStyle = dataGridViewCellStyle2;
            this.dataGridRWData.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dataGridRWData.Location = new System.Drawing.Point(0, 0);
            this.dataGridRWData.Name = "dataGridRWData";
            this.dataGridRWData.Size = new System.Drawing.Size(319, 435);
            this.dataGridRWData.TabIndex = 57;
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.tableLayoutPanel4);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Size = new System.Drawing.Size(637, 441);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "ROM";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // tableLayoutPanel4
            // 
            this.tableLayoutPanel4.ColumnCount = 2;
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 120F));
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel4.Controls.Add(this.dataGridFlashData, 1, 0);
            this.tableLayoutPanel4.Controls.Add(this.panel3, 0, 0);
            this.tableLayoutPanel4.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel4.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel4.Name = "tableLayoutPanel4";
            this.tableLayoutPanel4.RowCount = 1;
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel4.Size = new System.Drawing.Size(637, 441);
            this.tableLayoutPanel4.TabIndex = 0;
            // 
            // dataGridFlashData
            // 
            this.dataGridFlashData.AllowUserToAddRows = false;
            this.dataGridFlashData.AllowUserToDeleteRows = false;
            this.dataGridFlashData.AllowUserToResizeRows = false;
            this.dataGridFlashData.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            dataGridViewCellStyle3.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle3.BackColor = System.Drawing.SystemColors.Window;
            dataGridViewCellStyle3.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            dataGridViewCellStyle3.ForeColor = System.Drawing.SystemColors.ControlText;
            dataGridViewCellStyle3.SelectionBackColor = System.Drawing.SystemColors.Highlight;
            dataGridViewCellStyle3.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle3.WrapMode = System.Windows.Forms.DataGridViewTriState.False;
            this.dataGridFlashData.DefaultCellStyle = dataGridViewCellStyle3;
            this.dataGridFlashData.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dataGridFlashData.Location = new System.Drawing.Point(123, 3);
            this.dataGridFlashData.Name = "dataGridFlashData";
            this.dataGridFlashData.Size = new System.Drawing.Size(511, 435);
            this.dataGridFlashData.TabIndex = 57;
            // 
            // panel3
            // 
            this.panel3.Controls.Add(this.checkBoxWriteRomBuffer);
            this.panel3.Controls.Add(this.button3);
            this.panel3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel3.Location = new System.Drawing.Point(3, 3);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(114, 435);
            this.panel3.TabIndex = 0;
            // 
            // checkBoxWriteRomBuffer
            // 
            this.checkBoxWriteRomBuffer.AutoSize = true;
            this.checkBoxWriteRomBuffer.Location = new System.Drawing.Point(3, 72);
            this.checkBoxWriteRomBuffer.Name = "checkBoxWriteRomBuffer";
            this.checkBoxWriteRomBuffer.Size = new System.Drawing.Size(88, 17);
            this.checkBoxWriteRomBuffer.TabIndex = 67;
            this.checkBoxWriteRomBuffer.Text = "Записывать";
            this.checkBoxWriteRomBuffer.UseVisualStyleBackColor = true;
            this.checkBoxWriteRomBuffer.CheckedChanged += new System.EventHandler(this.checkBoxWriteRomBuffer_CheckedChanged);
            // 
            // button3
            // 
            this.button3.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.button3.ForeColor = System.Drawing.Color.DarkRed;
            this.button3.Location = new System.Drawing.Point(3, 0);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(108, 66);
            this.button3.TabIndex = 66;
            this.button3.Text = "ROM buffer to ROM";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button_Click_RomBufferToRom);
            // 
            // tabPage3
            // 
            this.tabPage3.Controls.Add(this.dataGridGeneralData);
            this.tabPage3.Location = new System.Drawing.Point(4, 22);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Size = new System.Drawing.Size(637, 441);
            this.tabPage3.TabIndex = 2;
            this.tabPage3.Text = "Базовая информация";
            this.tabPage3.UseVisualStyleBackColor = true;
            // 
            // dataGridGeneralData
            // 
            this.dataGridGeneralData.AllowUserToAddRows = false;
            this.dataGridGeneralData.AllowUserToDeleteRows = false;
            this.dataGridGeneralData.AllowUserToResizeRows = false;
            dataGridViewCellStyle4.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle4.BackColor = System.Drawing.SystemColors.Control;
            dataGridViewCellStyle4.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            dataGridViewCellStyle4.ForeColor = System.Drawing.SystemColors.WindowText;
            dataGridViewCellStyle4.SelectionBackColor = System.Drawing.SystemColors.Highlight;
            dataGridViewCellStyle4.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle4.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.dataGridGeneralData.ColumnHeadersDefaultCellStyle = dataGridViewCellStyle4;
            this.dataGridGeneralData.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            dataGridViewCellStyle5.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle5.BackColor = System.Drawing.SystemColors.Window;
            dataGridViewCellStyle5.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            dataGridViewCellStyle5.ForeColor = System.Drawing.SystemColors.ControlText;
            dataGridViewCellStyle5.SelectionBackColor = System.Drawing.SystemColors.Highlight;
            dataGridViewCellStyle5.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle5.WrapMode = System.Windows.Forms.DataGridViewTriState.False;
            this.dataGridGeneralData.DefaultCellStyle = dataGridViewCellStyle5;
            this.dataGridGeneralData.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dataGridGeneralData.Location = new System.Drawing.Point(0, 0);
            this.dataGridGeneralData.Name = "dataGridGeneralData";
            this.dataGridGeneralData.Size = new System.Drawing.Size(637, 441);
            this.dataGridGeneralData.TabIndex = 56;
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.DebugBox, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.panel2, 0, 0);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(3, 3);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 2;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 180F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(188, 467);
            this.tableLayoutPanel1.TabIndex = 65;
            // 
            // DebugBox
            // 
            this.DebugBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.DebugBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.DebugBox.ForeColor = System.Drawing.Color.DarkRed;
            this.DebugBox.Location = new System.Drawing.Point(3, 183);
            this.DebugBox.Multiline = true;
            this.DebugBox.Name = "DebugBox";
            this.DebugBox.Size = new System.Drawing.Size(182, 281);
            this.DebugBox.TabIndex = 33;
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.textBoxNetId);
            this.panel2.Controls.Add(this.label1);
            this.panel2.Controls.Add(this.labelCycleCounter);
            this.panel2.Controls.Add(this.button2);
            this.panel2.Controls.Add(this.comboBoxAvailableSpeeds);
            this.panel2.Controls.Add(this.comboBoxAvailablePortList);
            this.panel2.Controls.Add(this.buttonOpenClose);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel2.Location = new System.Drawing.Point(3, 3);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(182, 174);
            this.panel2.TabIndex = 0;
            // 
            // textBoxNetId
            // 
            this.textBoxNetId.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.textBoxNetId.Location = new System.Drawing.Point(102, 153);
            this.textBoxNetId.Name = "textBoxNetId";
            this.textBoxNetId.Size = new System.Drawing.Size(63, 24);
            this.textBoxNetId.TabIndex = 40;
            this.textBoxNetId.Text = "1";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Times New Roman", 12F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.label1.ForeColor = System.Drawing.Color.Black;
            this.label1.Location = new System.Drawing.Point(0, 155);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(92, 19);
            this.label1.TabIndex = 39;
            this.label1.Text = "Сетевой ID";
            // 
            // labelCycleCounter
            // 
            this.labelCycleCounter.AutoSize = true;
            this.labelCycleCounter.Font = new System.Drawing.Font("Times New Roman", 12F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.labelCycleCounter.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(64)))), ((int)(((byte)(0)))));
            this.labelCycleCounter.Location = new System.Drawing.Point(6, 110);
            this.labelCycleCounter.Name = "labelCycleCounter";
            this.labelCycleCounter.Size = new System.Drawing.Size(29, 19);
            this.labelCycleCounter.TabIndex = 37;
            this.labelCycleCounter.Text = "0/0";
            // 
            // button2
            // 
            this.button2.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.button2.Location = new System.Drawing.Point(98, 56);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(81, 51);
            this.button2.TabIndex = 36;
            this.button2.Text = "Очистить";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.clearDebugBox);
            // 
            // comboBoxAvailableSpeeds
            // 
            this.comboBoxAvailableSpeeds.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.comboBoxAvailableSpeeds.FormattingEnabled = true;
            this.comboBoxAvailableSpeeds.Location = new System.Drawing.Point(6, 83);
            this.comboBoxAvailableSpeeds.Name = "comboBoxAvailableSpeeds";
            this.comboBoxAvailableSpeeds.Size = new System.Drawing.Size(86, 24);
            this.comboBoxAvailableSpeeds.TabIndex = 38;
            this.comboBoxAvailableSpeeds.Text = "115200";
            // 
            // comboBoxAvailablePortList
            // 
            this.comboBoxAvailablePortList.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.comboBoxAvailablePortList.FormattingEnabled = true;
            this.comboBoxAvailablePortList.Location = new System.Drawing.Point(6, 56);
            this.comboBoxAvailablePortList.Name = "comboBoxAvailablePortList";
            this.comboBoxAvailablePortList.Size = new System.Drawing.Size(86, 24);
            this.comboBoxAvailablePortList.TabIndex = 3;
            this.comboBoxAvailablePortList.Click += new System.EventHandler(this.comboBoxAvailablePortList_Click);
            // 
            // buttonOpenClose
            // 
            this.buttonOpenClose.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.buttonOpenClose.Location = new System.Drawing.Point(6, 6);
            this.buttonOpenClose.Name = "buttonOpenClose";
            this.buttonOpenClose.Size = new System.Drawing.Size(173, 44);
            this.buttonOpenClose.TabIndex = 2;
            this.buttonOpenClose.Text = "Открыть";
            this.buttonOpenClose.UseVisualStyleBackColor = true;
            this.buttonOpenClose.Click += new System.EventHandler(this.buttonOpenClose_Click);
            // 
            // timer10ms
            // 
            this.timer10ms.Interval = 10;
            this.timer10ms.Tick += new System.EventHandler(this.timer10ms_Tick);
            // 
            // FormMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(845, 473);
            this.Controls.Add(this.tableLayoutPanel2);
            this.Name = "FormMain";
            this.Text = "Программа для работы с MiniModbus устройствами по последовательному порту";
            this.tableLayoutPanel2.ResumeLayout(false);
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridROData)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridRWData)).EndInit();
            this.tabPage2.ResumeLayout(false);
            this.tableLayoutPanel4.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridFlashData)).EndInit();
            this.panel3.ResumeLayout(false);
            this.panel3.PerformLayout();
            this.tabPage3.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridGeneralData)).EndInit();
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.Button buttonOpenClose;
        private System.Windows.Forms.ComboBox comboBoxAvailablePortList;
        private System.Windows.Forms.ComboBox comboBoxAvailableSpeeds;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Label labelCycleCounter;
        private System.Windows.Forms.TextBox DebugBox;
        private System.Windows.Forms.Timer timer10ms;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.DataGridView dataGridROData;
        private System.Windows.Forms.DataGridView dataGridRWData;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel4;
        private System.Windows.Forms.DataGridView dataGridFlashData;
        private System.Windows.Forms.Panel panel3;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.TabPage tabPage3;
        private System.Windows.Forms.DataGridView dataGridGeneralData;
        private System.Windows.Forms.TextBox textBoxNetId;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.CheckBox checkBoxWriteRomBuffer;
    }
}

