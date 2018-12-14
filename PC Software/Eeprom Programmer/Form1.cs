using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using System.Globalization;

namespace Eeprom_Programmer
{
    public partial class Form1 : Form
    {

        static byte[] IntToHex = { (byte)'0', (byte)'1', (byte)'2', (byte)'3', (byte)'4', (byte)'5', (byte)'6', (byte)'7', (byte)'8', (byte)'9', (byte)'A', (byte)'B', (byte)'C', (byte)'D', (byte)'E', (byte)'F' };
        static byte[] HexToInt = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 10, 11, 12, 13, 14, 15 };
        bool I2COku = false;

        delegate void SetTextCallback_string(string text);
        private void AddText(string text)
        {
            // InvokeRequired required compares the thread ID of the
            // calling thread to the thread ID of the creating thread.
            // If these threads are different, it returns true.
            if (this.richTextBox1.InvokeRequired)
            {
                SetTextCallback_string d = new SetTextCallback_string(AddText);
                this.Invoke(d, new object[] { text });
            }
            else
            {
                this.richTextBox1.Text += (text + "\n");
            }
        }
        delegate void SetNumeric(byte text);
        private void Numeric_Value(byte text)
        {
            // InvokeRequired required compares the thread ID of the
            // calling thread to the thread ID of the creating thread.
            // If these threads are different, it returns true.
            if (this.numericUpDown4.InvokeRequired)
            {
                SetNumeric d = new SetNumeric(Numeric_Value);
                this.Invoke(d, new object[] { text });
            }
            else
            {
                numericUpDown4.Value = text;
            }
        }
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            comboBox1.Items.AddRange(SerialPort.GetPortNames());
        }

        void Komut_yolla(string komut)
        {

            int Hesaplanan_CRC = 0x00;
            int SerialTxCount = 0;
            byte[] UartTXData = new byte[50];
            for (int i = 0; i < komut.Length; i++) UartTXData[i] = (byte)(Convert.ToChar(komut.Substring(i, 1)));
            UartTXData[komut.Length] = 0;
            SerialTxCount = 1;
            do
            {
                Hesaplanan_CRC ^= UartTXData[SerialTxCount++];
            } while (UartTXData[SerialTxCount] != 0);

            UartTXData[SerialTxCount++] = (byte)'*';
            UartTXData[SerialTxCount++] = IntToHex[Hesaplanan_CRC >> 4];
            UartTXData[SerialTxCount++] = IntToHex[Hesaplanan_CRC & 0x0F];
            UartTXData[SerialTxCount++] = (byte)'\r';
            UartTXData[SerialTxCount++] = (byte)'\n';
            UartTXData[SerialTxCount] = (byte)'\0';
            if (serialPort1.IsOpen)
            {
                AddText("<-" + komut);
                serialPort1.Write(UartTXData, 0, SerialTxCount + 1);
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            serialPort1.PortName = comboBox1.Text;
            serialPort1.Open();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Komut_yolla("$IY," + Convert.ToByte(numericUpDown1.Value).ToString("X2") + "," + Convert.ToByte(numericUpDown2.Value).ToString("X2") + "," + Convert.ToByte(numericUpDown3.Value).ToString("X2") + ",");
        }

        private void button3_Click(object sender, EventArgs e)
        {
            I2COku = true;
            Komut_yolla("$IO," + Convert.ToByte(numericUpDown1.Value).ToString("X2") + "," + Convert.ToByte(numericUpDown2.Value).ToString("X2") + ",");
        }

        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            string gelen = serialPort1.ReadLine();
            AddText("->" + gelen);

            if (gelen.Contains("$IO,") && I2COku)
            {
                I2COku = false;
                gelen = gelen.Replace("$IO,", "");
                Numeric_Value(byte.Parse(gelen.Substring(0, 2), NumberStyles.HexNumber));
            }
        }
    }
}
