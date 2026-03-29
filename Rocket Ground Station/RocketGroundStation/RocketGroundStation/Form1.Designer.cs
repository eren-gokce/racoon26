namespace RocketGroundStation
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
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
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            cmbPorts = new ComboBox();
            cmbBaudRates = new ComboBox();
            btnConnect = new Button();
            lstConsole = new ListBox();
            btnRefresh_Click = new Button();
            pnlRocketData = new Panel();
            lblSpeedTxt = new Label();
            lblGyroZTxt = new Label();
            lblGyroYTxt = new Label();
            lblGyroXTxt = new Label();
            lblAccelZTxt = new Label();
            lblAccelYTxt = new Label();
            lblAccelXTxt = new Label();
            lblLonTxt = new Label();
            lblLatTxt = new Label();
            lblGpsAltTxt = new Label();
            lblBaroAltTxt = new Label();
            lblSpeed = new Label();
            lblGyroZ = new Label();
            lblGyroY = new Label();
            lblGyroX = new Label();
            lblAccelZ = new Label();
            lblAccelY = new Label();
            lblAccelX = new Label();
            lblLon = new Label();
            lblLat = new Label();
            lblGpsAlt = new Label();
            lblBaroAlt = new Label();
            pnlRocketData.SuspendLayout();
            SuspendLayout();
            // 
            // cmbPorts
            // 
            cmbPorts.FormattingEnabled = true;
            cmbPorts.Location = new Point(115, 35);
            cmbPorts.Name = "cmbPorts";
            cmbPorts.Size = new Size(68, 28);
            cmbPorts.TabIndex = 0;
            // 
            // cmbBaudRates
            // 
            cmbBaudRates.FormattingEnabled = true;
            cmbBaudRates.Location = new Point(189, 35);
            cmbBaudRates.Name = "cmbBaudRates";
            cmbBaudRates.Size = new Size(75, 28);
            cmbBaudRates.TabIndex = 1;
            // 
            // btnConnect
            // 
            btnConnect.Location = new Point(270, 36);
            btnConnect.Name = "btnConnect";
            btnConnect.Size = new Size(94, 29);
            btnConnect.TabIndex = 2;
            btnConnect.Text = "Connect";
            btnConnect.UseVisualStyleBackColor = true;
            btnConnect.Click += btnConnect_Click;
            // 
            // lstConsole
            // 
            lstConsole.Font = new Font("Segoe UI", 8F);
            lstConsole.FormattingEnabled = true;
            lstConsole.ItemHeight = 17;
            lstConsole.Location = new Point(67, 71);
            lstConsole.Name = "lstConsole";
            lstConsole.Size = new Size(586, 140);
            lstConsole.TabIndex = 3;
            // 
            // btnRefresh_Click
            // 
            btnRefresh_Click.BackColor = Color.Transparent;
            btnRefresh_Click.Image = Properties.Resources.arrows_rotate_solid_full;
            btnRefresh_Click.Location = new Point(76, 31);
            btnRefresh_Click.Name = "btnRefresh_Click";
            btnRefresh_Click.RightToLeft = RightToLeft.No;
            btnRefresh_Click.Size = new Size(33, 34);
            btnRefresh_Click.TabIndex = 0;
            btnRefresh_Click.UseVisualStyleBackColor = false;
            btnRefresh_Click.Click += btnRefresh;
            // 
            // pnlRocketData
            // 
            pnlRocketData.Controls.Add(lblSpeedTxt);
            pnlRocketData.Controls.Add(lblGyroZTxt);
            pnlRocketData.Controls.Add(lblGyroYTxt);
            pnlRocketData.Controls.Add(lblGyroXTxt);
            pnlRocketData.Controls.Add(lblAccelZTxt);
            pnlRocketData.Controls.Add(lblAccelYTxt);
            pnlRocketData.Controls.Add(lblAccelXTxt);
            pnlRocketData.Controls.Add(lblLonTxt);
            pnlRocketData.Controls.Add(lblLatTxt);
            pnlRocketData.Controls.Add(lblGpsAltTxt);
            pnlRocketData.Controls.Add(lblBaroAltTxt);
            pnlRocketData.Controls.Add(lblSpeed);
            pnlRocketData.Controls.Add(lblGyroZ);
            pnlRocketData.Controls.Add(lblGyroY);
            pnlRocketData.Controls.Add(lblGyroX);
            pnlRocketData.Controls.Add(lblAccelZ);
            pnlRocketData.Controls.Add(lblAccelY);
            pnlRocketData.Controls.Add(lblAccelX);
            pnlRocketData.Controls.Add(lblLon);
            pnlRocketData.Controls.Add(lblLat);
            pnlRocketData.Controls.Add(lblGpsAlt);
            pnlRocketData.Controls.Add(lblBaroAlt);
            pnlRocketData.Location = new Point(690, 71);
            pnlRocketData.Name = "pnlRocketData";
            pnlRocketData.Size = new Size(205, 280);
            pnlRocketData.TabIndex = 5;
            // 
            // lblSpeedTxt
            // 
            lblSpeedTxt.AutoSize = true;
            lblSpeedTxt.Location = new Point(28, 224);
            lblSpeedTxt.Name = "lblSpeedTxt";
            lblSpeedTxt.Size = new Size(51, 20);
            lblSpeedTxt.TabIndex = 0;
            lblSpeedTxt.Text = "Speed";
            // 
            // lblGyroZTxt
            // 
            lblGyroZTxt.AutoSize = true;
            lblGyroZTxt.Location = new Point(28, 204);
            lblGyroZTxt.Name = "lblGyroZTxt";
            lblGyroZTxt.Size = new Size(49, 20);
            lblGyroZTxt.TabIndex = 0;
            lblGyroZTxt.Text = "GyroZ";
            // 
            // lblGyroYTxt
            // 
            lblGyroYTxt.AutoSize = true;
            lblGyroYTxt.Location = new Point(28, 184);
            lblGyroYTxt.Name = "lblGyroYTxt";
            lblGyroYTxt.Size = new Size(48, 20);
            lblGyroYTxt.TabIndex = 0;
            lblGyroYTxt.Text = "GyroY";
            // 
            // lblGyroXTxt
            // 
            lblGyroXTxt.AutoSize = true;
            lblGyroXTxt.Location = new Point(28, 164);
            lblGyroXTxt.Name = "lblGyroXTxt";
            lblGyroXTxt.Size = new Size(49, 20);
            lblGyroXTxt.TabIndex = 0;
            lblGyroXTxt.Text = "GyroX";
            // 
            // lblAccelZTxt
            // 
            lblAccelZTxt.AutoSize = true;
            lblAccelZTxt.Location = new Point(28, 144);
            lblAccelZTxt.Name = "lblAccelZTxt";
            lblAccelZTxt.Size = new Size(54, 20);
            lblAccelZTxt.TabIndex = 0;
            lblAccelZTxt.Text = "AccelZ";
            // 
            // lblAccelYTxt
            // 
            lblAccelYTxt.AutoSize = true;
            lblAccelYTxt.Location = new Point(28, 124);
            lblAccelYTxt.Name = "lblAccelYTxt";
            lblAccelYTxt.Size = new Size(53, 20);
            lblAccelYTxt.TabIndex = 0;
            lblAccelYTxt.Text = "AccelY";
            // 
            // lblAccelXTxt
            // 
            lblAccelXTxt.AutoSize = true;
            lblAccelXTxt.Location = new Point(28, 104);
            lblAccelXTxt.Name = "lblAccelXTxt";
            lblAccelXTxt.Size = new Size(54, 20);
            lblAccelXTxt.TabIndex = 0;
            lblAccelXTxt.Text = "AccelX";
            lblAccelXTxt.Click += lblAccelXTxt_Click;
            // 
            // lblLonTxt
            // 
            lblLonTxt.AutoSize = true;
            lblLonTxt.Location = new Point(28, 84);
            lblLonTxt.Name = "lblLonTxt";
            lblLonTxt.Size = new Size(33, 20);
            lblLonTxt.TabIndex = 0;
            lblLonTxt.Text = "Lon";
            // 
            // lblLatTxt
            // 
            lblLatTxt.AutoSize = true;
            lblLatTxt.Location = new Point(28, 64);
            lblLatTxt.Name = "lblLatTxt";
            lblLatTxt.Size = new Size(29, 20);
            lblLatTxt.TabIndex = 0;
            lblLatTxt.Text = "Lat";
            // 
            // lblGpsAltTxt
            // 
            lblGpsAltTxt.AutoSize = true;
            lblGpsAltTxt.Location = new Point(28, 44);
            lblGpsAltTxt.Name = "lblGpsAltTxt";
            lblGpsAltTxt.Size = new Size(53, 20);
            lblGpsAltTxt.TabIndex = 0;
            lblGpsAltTxt.Text = "GpsAlt";
            // 
            // lblBaroAltTxt
            // 
            lblBaroAltTxt.AutoSize = true;
            lblBaroAltTxt.Location = new Point(28, 24);
            lblBaroAltTxt.Name = "lblBaroAltTxt";
            lblBaroAltTxt.Size = new Size(59, 20);
            lblBaroAltTxt.TabIndex = 0;
            lblBaroAltTxt.Text = "BaroAlt";
            // 
            // lblSpeed
            // 
            lblSpeed.AutoSize = true;
            lblSpeed.Location = new Point(120, 224);
            lblSpeed.Name = "lblSpeed";
            lblSpeed.Size = new Size(17, 20);
            lblSpeed.TabIndex = 0;
            lblSpeed.Text = "0";
            // 
            // lblGyroZ
            // 
            lblGyroZ.AutoSize = true;
            lblGyroZ.Location = new Point(120, 204);
            lblGyroZ.Name = "lblGyroZ";
            lblGyroZ.Size = new Size(17, 20);
            lblGyroZ.TabIndex = 0;
            lblGyroZ.Text = "0";
            // 
            // lblGyroY
            // 
            lblGyroY.AutoSize = true;
            lblGyroY.Location = new Point(120, 184);
            lblGyroY.Name = "lblGyroY";
            lblGyroY.Size = new Size(17, 20);
            lblGyroY.TabIndex = 0;
            lblGyroY.Text = "0";
            // 
            // lblGyroX
            // 
            lblGyroX.AutoSize = true;
            lblGyroX.Location = new Point(120, 164);
            lblGyroX.Name = "lblGyroX";
            lblGyroX.Size = new Size(17, 20);
            lblGyroX.TabIndex = 0;
            lblGyroX.Text = "0";
            // 
            // lblAccelZ
            // 
            lblAccelZ.AutoSize = true;
            lblAccelZ.Location = new Point(120, 144);
            lblAccelZ.Name = "lblAccelZ";
            lblAccelZ.Size = new Size(17, 20);
            lblAccelZ.TabIndex = 0;
            lblAccelZ.Text = "0";
            // 
            // lblAccelY
            // 
            lblAccelY.AutoSize = true;
            lblAccelY.Location = new Point(120, 124);
            lblAccelY.Name = "lblAccelY";
            lblAccelY.Size = new Size(17, 20);
            lblAccelY.TabIndex = 0;
            lblAccelY.Text = "0";
            // 
            // lblAccelX
            // 
            lblAccelX.AutoSize = true;
            lblAccelX.Location = new Point(120, 104);
            lblAccelX.Name = "lblAccelX";
            lblAccelX.Size = new Size(17, 20);
            lblAccelX.TabIndex = 0;
            lblAccelX.Text = "0";
            // 
            // lblLon
            // 
            lblLon.AutoSize = true;
            lblLon.Location = new Point(120, 84);
            lblLon.Name = "lblLon";
            lblLon.Size = new Size(17, 20);
            lblLon.TabIndex = 0;
            lblLon.Text = "0";
            // 
            // lblLat
            // 
            lblLat.AutoSize = true;
            lblLat.Location = new Point(120, 64);
            lblLat.Name = "lblLat";
            lblLat.Size = new Size(17, 20);
            lblLat.TabIndex = 0;
            lblLat.Text = "0";
            // 
            // lblGpsAlt
            // 
            lblGpsAlt.AutoSize = true;
            lblGpsAlt.Location = new Point(120, 44);
            lblGpsAlt.Name = "lblGpsAlt";
            lblGpsAlt.Size = new Size(17, 20);
            lblGpsAlt.TabIndex = 0;
            lblGpsAlt.Text = "0";
            // 
            // lblBaroAlt
            // 
            lblBaroAlt.AutoSize = true;
            lblBaroAlt.Location = new Point(120, 24);
            lblBaroAlt.Name = "lblBaroAlt";
            lblBaroAlt.Size = new Size(17, 20);
            lblBaroAlt.TabIndex = 0;
            lblBaroAlt.Text = "0";
            // 
            // Form1
            // 
            AccessibleRole = AccessibleRole.TitleBar;
            AutoScaleDimensions = new SizeF(8F, 20F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(1902, 953);
            Controls.Add(pnlRocketData);
            Controls.Add(btnRefresh_Click);
            Controls.Add(lstConsole);
            Controls.Add(btnConnect);
            Controls.Add(cmbBaudRates);
            Controls.Add(cmbPorts);
            Icon = (Icon)resources.GetObject("$this.Icon");
            Name = "Form1";
            Text = "Rocket GUI";
            FormClosing += Form1_FormClosing;
            Load += Form1_Load;
            pnlRocketData.ResumeLayout(false);
            pnlRocketData.PerformLayout();
            ResumeLayout(false);
        }

        #endregion

        private ComboBox cmbPorts;
        private ComboBox cmbBaudRates;
        private Button btnConnect;
        private ListBox lstConsole;
        private Button btnRefresh_Click;
        private Panel pnlRocketData;
        private Label lblBaroAlt;
        private Label lblGpsAltTxt;
        private Label lblBaroAltTxt;
        private Label lblGpsAlt;
        private Label lblLatTxt;
        private Label lblLat;
        private Label lblLonTxt;
        private Label lblLon;
        private Label lblAccelYTxt;
        private Label lblAccelXTxt;
        private Label lblAccelZTxt;
        private Label lblGyroZTxt;
        private Label lblGyroYTxt;
        private Label lblGyroXTxt;
        private Label lblSpeedTxt;
        private Label lblSpeed;
        private Label lblGyroZ;
        private Label lblGyroY;
        private Label lblGyroX;
        private Label lblAccelZ;
        private Label lblAccelY;
        private Label lblAccelX;
    }
}
