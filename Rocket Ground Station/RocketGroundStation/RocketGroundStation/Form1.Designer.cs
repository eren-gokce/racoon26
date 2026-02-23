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
            lstConsole.Size = new Size(1106, 276);
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
            // Form1
            // 
            AccessibleRole = AccessibleRole.TitleBar;
            AutoScaleDimensions = new SizeF(8F, 20F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(1902, 953);
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
            ResumeLayout(false);
        }

        #endregion

        private ComboBox cmbPorts;
        private ComboBox cmbBaudRates;
        private Button btnConnect;
        private ListBox lstConsole;
        private Button btnRefresh_Click;
    }
}
