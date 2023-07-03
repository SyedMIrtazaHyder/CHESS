namespace WindowsFormsApp1
{
    partial class Form1
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
            this.Turn = new System.Windows.Forms.Label();
            this.ErrorScreen = new System.Windows.Forms.TextBox();
            this.Board = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.Board)).BeginInit();
            this.SuspendLayout();
            // 
            // Turn
            // 
            this.Turn.AutoSize = true;
            this.Turn.BackColor = System.Drawing.Color.Black;
            this.Turn.Font = new System.Drawing.Font("Rockwell", 13.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Turn.ForeColor = System.Drawing.Color.White;
            this.Turn.Location = new System.Drawing.Point(658, 18);
            this.Turn.Name = "Turn";
            this.Turn.Size = new System.Drawing.Size(146, 28);
            this.Turn.TabIndex = 1;
            this.Turn.Text = "Turn: White";
            // 
            // ErrorScreen
            // 
            this.ErrorScreen.Cursor = System.Windows.Forms.Cursors.No;
            this.ErrorScreen.Font = new System.Drawing.Font("Rockwell", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ErrorScreen.ForeColor = System.Drawing.SystemColors.WindowText;
            this.ErrorScreen.Location = new System.Drawing.Point(654, 74);
            this.ErrorScreen.Multiline = true;
            this.ErrorScreen.Name = "ErrorScreen";
            this.ErrorScreen.ReadOnly = true;
            this.ErrorScreen.Size = new System.Drawing.Size(284, 52);
            this.ErrorScreen.TabIndex = 3;
            // 
            // Board
            // 
            this.Board.BackColor = System.Drawing.Color.Transparent;
            this.Board.Image = global::WindowsFormsApp1.Properties.Resources.chessboard;
            this.Board.Location = new System.Drawing.Point(35, 10);
            this.Board.Name = "Board";
            this.Board.Size = new System.Drawing.Size(600, 600);
            this.Board.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.Board.TabIndex = 0;
            this.Board.TabStop = false;
            this.Board.Click += new System.EventHandler(this.MovePiece);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(950, 591);
            this.Controls.Add(this.ErrorScreen);
            this.Controls.Add(this.Turn);
            this.Controls.Add(this.Board);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.Board)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox Board;
        private System.Windows.Forms.Label Turn;
        private System.Windows.Forms.TextBox ErrorScreen;
    }
}

