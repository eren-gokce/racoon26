using System.IO.Ports;
using System.Runtime.InteropServices;

namespace RocketGroundStation
{
    public partial class Form1 : Form
    {
        private SerialPort _serialPort;
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            // 1. Bilgisayara bağlı aktif seri portları çek
            try
            {
                string[] ports = SerialPort.GetPortNames();
                cmbPorts.Items.AddRange(ports);

                // Eğer port bulunduysa otomatik olarak ilkini seç
                if (cmbPorts.Items.Count > 0)
                {
                    cmbPorts.SelectedIndex = 0;
                }
            }
            catch (Exception ex)
            {
                LogToConsole("Portlar listelenirken hata oluştu: " + ex.Message);
            }

            // 2. Standart Baud Rate değerlerini ekle
            // Genelde LoRa modülleri ve ESP32 için 9600 veya 115200 kullanılır.
            cmbBaudRates.Items.Add("2400");
            cmbBaudRates.Items.Add("4800");
            cmbBaudRates.Items.Add("9600");
            cmbBaudRates.Items.Add("19200");
            cmbBaudRates.Items.Add("38400");
            cmbBaudRates.Items.Add("57600");
            cmbBaudRates.Items.Add("115200");

            // Varsayılan olarak 115200 seçili gelsin (Genelde LoRa default budur)
            cmbBaudRates.SelectedIndex = 6;
        }

        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct RocketDataPacket
        {
            public byte id;         // uint8_t
            public float baroAlt;   // float
            public float gpsAlt;    // float
            public float lat;       // float
            public float lon;       // float
            public float accelX;    // float
            public float accelY;    // float
            public float accelZ;    // float
            public float gyroX;     // float
            public float gyroY;     // float
            public float gyroZ;     // float
            public float speed;     // float
            public byte status;     // uint8_t
            public byte crc;        // uint8_t
        }

        private void LogToConsole(string message)
        {
            // 1. Thread Safety Kontrolü: 
            // Seri port verisi arka plandaki bir thread'den gelir. 
            // UI elemanlarına (ListBox) dışarıdan müdahale etmek uygulamayı çökertir.
            if (lstConsole.InvokeRequired)
            {
                // Eğer başka bir thread'den geliyorsa, işlemi ana thread'e (UI thread) aktar.
                lstConsole.Invoke(new Action<string>(LogToConsole), message);
            }
            else
            {
                // 2. Mesaj Formatlama:
                // Başına zaman damgası ekleyerek daha profesyonel bir log yapısı kuruyoruz.
                string logEntry = $"[{DateTime.Now:HH:mm:ss}] {message}";

                // 3. ListBox'a Ekleme:
                lstConsole.Items.Add(logEntry);

                // 4. Auto-Scroll (Otomatik Kaydırma):
                // Yeni veri geldiğinde listenin en altına odaklanmasını sağlar.
                lstConsole.TopIndex = lstConsole.Items.Count - 1;

                // Satır sayısı 23'ü geçerse, en eski (ilk) satırı sil
                while (lstConsole.Items.Count > 16)
                {
                    lstConsole.Items.RemoveAt(0);
                }

                // 5. Bellek Yönetimi (Opsiyonel ama Tavsiye Edilir):
                // Konsol çok şişerse uygulamanın yavaşlamaması için eski kayıtları silebiliriz.
                if (lstConsole.Items.Count > 500)
                {
                    lstConsole.Items.RemoveAt(0);
                }
            }
        }

        private RocketDataPacket ByteArrayToStruct(byte[] bytes)
        {
            // 1. Gelen byte dizisinin uzunluğunu kontrol et (Senin paket 47 byte)
            // Eğer uzunluk tutmazsa hata vermemesi için kontrol ekliyoruz.
            if (bytes.Length < Marshal.SizeOf(typeof(RocketDataPacket)))
            {
                return new RocketDataPacket();
            }

            // 2. Byte dizisini bellekte sabitle (Garbage Collector yerini değiştirmesin)
            GCHandle handle = GCHandle.Alloc(bytes, GCHandleType.Pinned);
            try
            {
                // 3. Bellekteki bu ham adresi al ve RocketDataPacket yapısına "giydir"
                return (RocketDataPacket)Marshal.PtrToStructure(handle.AddrOfPinnedObject(), typeof(RocketDataPacket));
            }
            finally
            {
                // 4. Bellek sabitlemesini kaldır (Bellek sızıntısını önlemek için kritik!)
                handle.Free();
            }
        }

        private void DataReceivedHandler(object sender, SerialDataReceivedEventArgs e)
        {
            // Seri portta veri olduğu sürece döngü devam eder
            while (_serialPort.BytesToRead > 0)
            {
                // 1. ID (151) gelene kadar byte'ları teker teker oku ve ele
                if (_serialPort.ReadByte() == 151)
                {
                    // ID bulundu! Şimdi paketin geri kalan 46 byte'ının gelmesini bekle
                    // (Toplam 47 byte: 1 ID + 46 Data)

                    int timeoutCounter = 0;
                    while (_serialPort.BytesToRead < 46 && timeoutCounter < 100)
                    {
                        Thread.Sleep(1); // Verinin yolda olması ihtimaline karşı çok kısa bekle
                        timeoutCounter++;
                    }

                    if (_serialPort.BytesToRead >= 46)
                    {
                        byte[] payload = new byte[47];
                        payload[0] = 151; // ID'yi manuel ekliyoruz çünkü az önce okuduk

                        // Geri kalan 46 byte'ı buffer'a çek
                        _serialPort.Read(payload, 1, 46);

                        // --- HAM VERİ LOGLAMA BAŞLANGIÇ ---
                        // Byte dizisini yan yana "97-00-00-A1..." şeklinde bir metne çevirir
                        string rawData = BitConverter.ToString(payload);
                        LogToConsole("RAW DATA: " + rawData);
                        // --- HAM VERİ LOGLAMA BİTİŞ ---

                        // 2. Struct'a dönüştür ve UI'ı güncelle
                        RocketDataPacket packet = ByteArrayToStruct(payload);

                        //this.Invoke(new Action(() => {
                        //    UpdateUI(packet);
                        //}));

                        //// Bağımsız görevleri tetikle
                        //Task.Run(() => Update2DMap(packet.lat.ToString(), packet.lon.ToString()));
                    }
                    // Eğer 46 byte tamamlanmazsa (timeout), bu paket hatalıdır; 
                    // döngü başa döner ve bir sonraki 151'i aramaya başlar.
                }
            }
        }

        private void btnConnect_Click(object sender, EventArgs e)
        {
            if (_serialPort == null || !_serialPort.IsOpen)
            {
                try
                {
                    _serialPort = new SerialPort();
                    _serialPort.PortName = cmbPorts.SelectedItem.ToString();
                    _serialPort.BaudRate = int.Parse(cmbBaudRates.SelectedItem.ToString());

                    // Veri geldiğinde ana thread'i yormadan çalışacak event
                    _serialPort.DataReceived += new SerialDataReceivedEventHandler(DataReceivedHandler);

                    _serialPort.Open();
                    btnConnect.Text = "Disconnect";
                    btnConnect.BackColor = Color.Red;
                    LogToConsole("Connected to " + _serialPort.PortName);
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Error: " + ex.Message);
                }
            }
            else
            {
                _serialPort.Close();
                btnConnect.Text = "Connect";
                btnConnect.BackColor = Color.Green;
                LogToConsole("Disconnected.");
            }
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (_serialPort != null && _serialPort.IsOpen)
            {
                _serialPort.Close();
            }
        }

        private void btnRefresh(object sender, EventArgs e)
        {
            // Mevcut seçili portu unutmamak için kaydedelim
            string currentPort = cmbPorts.SelectedItem?.ToString();

            cmbPorts.Items.Clear();
            string[] ports = SerialPort.GetPortNames();
            cmbPorts.Items.AddRange(ports);

            if (cmbPorts.Items.Count > 0)
            {
                // Eğer eski seçili port hala listedeyse onu tekrar seç, yoksa ilkini seç
                if (currentPort != null && ports.Contains(currentPort))
                    cmbPorts.SelectedItem = currentPort;
                else
                    cmbPorts.SelectedIndex = 0;
            }

            LogToConsole("Port listesi güncellendi. Bulunan port sayısı: " + ports.Length);
        }
    }
}
