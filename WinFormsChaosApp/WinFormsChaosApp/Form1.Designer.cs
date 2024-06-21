namespace WinFormsChaosApp
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
            button1 = new Button();
            button2 = new Button();
            button3 = new Button();
            button4 = new Button();
            button5 = new Button();
            button6 = new Button();
            SuspendLayout();
            // 
            // button1
            // 
            button1.Location = new Point(12, 304);
            button1.Name = "button1";
            button1.Size = new Size(146, 38);
            button1.TabIndex = 0;
            button1.Text = "Open";
            button1.UseVisualStyleBackColor = true;
            button1.Click += OpenClick;
            // 
            // button2
            // 
            button2.Location = new Point(18, 348);
            button2.Name = "button2";
            button2.Size = new Size(140, 39);
            button2.TabIndex = 1;
            button2.Text = "Save";
            button2.UseVisualStyleBackColor = true;
            button2.Click += SaveClick;
            // 
            // button3
            // 
            button3.Location = new Point(182, 304);
            button3.Name = "button3";
            button3.Size = new Size(140, 39);
            button3.TabIndex = 2;
            button3.Text = "GMeta";
            button3.UseVisualStyleBackColor = true;
            button3.Click += GetMetaClick;
            // 
            // button4
            // 
            button4.Location = new Point(182, 349);
            button4.Name = "button4";
            button4.Size = new Size(140, 39);
            button4.TabIndex = 3;
            button4.Text = "SMeta";
            button4.UseVisualStyleBackColor = true;
            button4.Click += SetMetaClick;
            // 
            // button5
            // 
            button5.Location = new Point(353, 305);
            button5.Name = "button5";
            button5.Size = new Size(131, 38);
            button5.TabIndex = 4;
            button5.Text = "Plugin";
            button5.UseVisualStyleBackColor = true;
            button5.Click += PluginClick;
            // 
            // button6
            // 
            button6.Location = new Point(353, 350);
            button6.Name = "button6";
            button6.Size = new Size(131, 38);
            button6.TabIndex = 5;
            button6.Text = "Container";
            button6.UseVisualStyleBackColor = true;
            button6.Click += ContainerClick;
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(800, 450);
            Controls.Add(button6);
            Controls.Add(button5);
            Controls.Add(button4);
            Controls.Add(button3);
            Controls.Add(button2);
            Controls.Add(button1);
            Name = "Form1";
            Text = "Form1";
            ResumeLayout(false);
        }

        #endregion

        private Button button1;
        private Button button2;
        private Button button3;
        private Button button4;
        private Button button5;
        private Button button6;
    }
}
