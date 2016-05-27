namespace PackedSerialApp
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
            this.buttonOpenClose = new System.Windows.Forms.Button();
            this.comboBoxAvailablePortList = new System.Windows.Forms.ComboBox();
            this.comboBoxAvailableSpeeds = new System.Windows.Forms.ComboBox();
            this.groupBoxPortSettings = new System.Windows.Forms.GroupBox();
            this.tableLayoutPanelMain = new System.Windows.Forms.TableLayoutPanel();
            this.flowLayoutPanelLeft = new System.Windows.Forms.FlowLayoutPanel();
            this.groupBoxOutputSettings = new System.Windows.Forms.GroupBox();
            this.buttonAddSendPanel = new System.Windows.Forms.Button();
            this.buttonClearPrint = new System.Windows.Forms.Button();
            this.checkBoxHEXFormat = new System.Windows.Forms.CheckBox();
            this.textBoxPrint = new System.Windows.Forms.TextBox();
            this.timer10ms = new System.Windows.Forms.Timer(this.components);
            this.groupBoxPortSettings.SuspendLayout();
            this.tableLayoutPanelMain.SuspendLayout();
            this.flowLayoutPanelLeft.SuspendLayout();
            this.groupBoxOutputSettings.SuspendLayout();
            this.SuspendLayout();
            // 
            // buttonOpenClose
            // 
            this.buttonOpenClose.Font = new System.Drawing.Font("Times New Roman", 12F);
            this.buttonOpenClose.Location = new System.Drawing.Point(6, 52);
            this.buttonOpenClose.Name = "buttonOpenClose";
            this.buttonOpenClose.Size = new System.Drawing.Size(249, 42);
            this.buttonOpenClose.TabIndex = 1;
            this.buttonOpenClose.Text = "Открыть";
            this.buttonOpenClose.UseVisualStyleBackColor = true;
            this.buttonOpenClose.Click += new System.EventHandler(this.buttonOpenClose_Click);
            // 
            // comboBoxAvailablePortList
            // 
            this.comboBoxAvailablePortList.Font = new System.Drawing.Font("Times New Roman", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.comboBoxAvailablePortList.FormattingEnabled = true;
            this.comboBoxAvailablePortList.Location = new System.Drawing.Point(6, 19);
            this.comboBoxAvailablePortList.Name = "comboBoxAvailablePortList";
            this.comboBoxAvailablePortList.Size = new System.Drawing.Size(114, 27);
            this.comboBoxAvailablePortList.TabIndex = 4;
            this.comboBoxAvailablePortList.Click += new System.EventHandler(this.comboBoxAvailablePortList_Click);
            // 
            // comboBoxAvailableSpeeds
            // 
            this.comboBoxAvailableSpeeds.Font = new System.Drawing.Font("Times New Roman", 12F);
            this.comboBoxAvailableSpeeds.FormattingEnabled = true;
            this.comboBoxAvailableSpeeds.Location = new System.Drawing.Point(126, 19);
            this.comboBoxAvailableSpeeds.Name = "comboBoxAvailableSpeeds";
            this.comboBoxAvailableSpeeds.Size = new System.Drawing.Size(129, 27);
            this.comboBoxAvailableSpeeds.TabIndex = 5;
            // 
            // groupBoxPortSettings
            // 
            this.groupBoxPortSettings.Controls.Add(this.comboBoxAvailablePortList);
            this.groupBoxPortSettings.Controls.Add(this.comboBoxAvailableSpeeds);
            this.groupBoxPortSettings.Controls.Add(this.buttonOpenClose);
            this.groupBoxPortSettings.Location = new System.Drawing.Point(3, 3);
            this.groupBoxPortSettings.Name = "groupBoxPortSettings";
            this.groupBoxPortSettings.Size = new System.Drawing.Size(261, 100);
            this.groupBoxPortSettings.TabIndex = 7;
            this.groupBoxPortSettings.TabStop = false;
            this.groupBoxPortSettings.Text = "Настройки порта";
            // 
            // tableLayoutPanelMain
            // 
            this.tableLayoutPanelMain.ColumnCount = 2;
            this.tableLayoutPanelMain.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 270F));
            this.tableLayoutPanelMain.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanelMain.Controls.Add(this.flowLayoutPanelLeft, 0, 0);
            this.tableLayoutPanelMain.Controls.Add(this.textBoxPrint, 1, 0);
            this.tableLayoutPanelMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanelMain.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanelMain.Name = "tableLayoutPanelMain";
            this.tableLayoutPanelMain.RowCount = 1;
            this.tableLayoutPanelMain.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanelMain.Size = new System.Drawing.Size(741, 424);
            this.tableLayoutPanelMain.TabIndex = 8;
            // 
            // flowLayoutPanelLeft
            // 
            this.flowLayoutPanelLeft.Controls.Add(this.groupBoxPortSettings);
            this.flowLayoutPanelLeft.Controls.Add(this.groupBoxOutputSettings);
            this.flowLayoutPanelLeft.Dock = System.Windows.Forms.DockStyle.Fill;
            this.flowLayoutPanelLeft.Location = new System.Drawing.Point(3, 3);
            this.flowLayoutPanelLeft.Name = "flowLayoutPanelLeft";
            this.flowLayoutPanelLeft.Size = new System.Drawing.Size(264, 418);
            this.flowLayoutPanelLeft.TabIndex = 0;
            // 
            // groupBoxOutputSettings
            // 
            this.groupBoxOutputSettings.Controls.Add(this.buttonAddSendPanel);
            this.groupBoxOutputSettings.Controls.Add(this.buttonClearPrint);
            this.groupBoxOutputSettings.Controls.Add(this.checkBoxHEXFormat);
            this.groupBoxOutputSettings.Location = new System.Drawing.Point(3, 109);
            this.groupBoxOutputSettings.Name = "groupBoxOutputSettings";
            this.groupBoxOutputSettings.Size = new System.Drawing.Size(255, 61);
            this.groupBoxOutputSettings.TabIndex = 9;
            this.groupBoxOutputSettings.TabStop = false;
            this.groupBoxOutputSettings.Text = "Настройка вывода";
            // 
            // buttonAddSendPanel
            // 
            this.buttonAddSendPanel.Location = new System.Drawing.Point(6, 19);
            this.buttonAddSendPanel.Name = "buttonAddSendPanel";
            this.buttonAddSendPanel.Size = new System.Drawing.Size(46, 35);
            this.buttonAddSendPanel.TabIndex = 2;
            this.buttonAddSendPanel.Text = "+send";
            this.buttonAddSendPanel.UseVisualStyleBackColor = true;
            this.buttonAddSendPanel.Click += new System.EventHandler(this.buttonAddSendPanel_Click);
            // 
            // buttonClearPrint
            // 
            this.buttonClearPrint.Font = new System.Drawing.Font("Times New Roman", 12F);
            this.buttonClearPrint.Location = new System.Drawing.Point(139, 19);
            this.buttonClearPrint.Name = "buttonClearPrint";
            this.buttonClearPrint.Size = new System.Drawing.Size(110, 35);
            this.buttonClearPrint.TabIndex = 1;
            this.buttonClearPrint.Text = "Очистить";
            this.buttonClearPrint.UseVisualStyleBackColor = true;
            this.buttonClearPrint.Click += new System.EventHandler(this.buttonClearPrint_Click);
            // 
            // checkBoxHEXFormat
            // 
            this.checkBoxHEXFormat.AutoSize = true;
            this.checkBoxHEXFormat.Location = new System.Drawing.Point(72, 29);
            this.checkBoxHEXFormat.Name = "checkBoxHEXFormat";
            this.checkBoxHEXFormat.Size = new System.Drawing.Size(48, 17);
            this.checkBoxHEXFormat.TabIndex = 0;
            this.checkBoxHEXFormat.Text = "HEX";
            this.checkBoxHEXFormat.UseVisualStyleBackColor = true;
            // 
            // textBoxPrint
            // 
            this.textBoxPrint.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBoxPrint.Font = new System.Drawing.Font("Consolas", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.textBoxPrint.Location = new System.Drawing.Point(273, 3);
            this.textBoxPrint.Multiline = true;
            this.textBoxPrint.Name = "textBoxPrint";
            this.textBoxPrint.Size = new System.Drawing.Size(465, 418);
            this.textBoxPrint.TabIndex = 1;
            // 
            // timer10ms
            // 
            this.timer10ms.Interval = 10;
            // 
            // FormMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(741, 424);
            this.Controls.Add(this.tableLayoutPanelMain);
            this.Name = "FormMain";
            this.Text = "Универсальная отладочная прогамма v2.0";
            this.groupBoxPortSettings.ResumeLayout(false);
            this.tableLayoutPanelMain.ResumeLayout(false);
            this.tableLayoutPanelMain.PerformLayout();
            this.flowLayoutPanelLeft.ResumeLayout(false);
            this.groupBoxOutputSettings.ResumeLayout(false);
            this.groupBoxOutputSettings.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button buttonOpenClose;
        private System.Windows.Forms.ComboBox comboBoxAvailablePortList;
        private System.Windows.Forms.ComboBox comboBoxAvailableSpeeds;
        private System.Windows.Forms.GroupBox groupBoxPortSettings;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanelMain;
        private System.Windows.Forms.FlowLayoutPanel flowLayoutPanelLeft;
        private System.Windows.Forms.GroupBox groupBoxOutputSettings;
        private System.Windows.Forms.TextBox textBoxPrint;
        private System.Windows.Forms.CheckBox checkBoxHEXFormat;
        private System.Windows.Forms.Button buttonClearPrint;
        private System.Windows.Forms.Timer timer10ms;
        private System.Windows.Forms.Button buttonAddSendPanel;
    }
}

