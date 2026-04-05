//main.ino
#include <Wire.h>
#include <Adafruit_BME280.h>
#include "functions.h"
#include <LoRa_E32.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <EEPROM.h> // flasha veri yazma
#include "esp_task_wdt.h" // Task watchdog timer

volatile bool switch_state = LOW; // yukseklik kaydetme  icin switch

#include <Ticker.h>
float altitude;
void first_flag_function(){
  yukseklikraw = altitude;
}
Ticker t;

unsigned long lora_timer;

// diger nesneler
HardwareSerial  SerialAT(2);
LoRa_E32        e32ttl(&SerialAT);
HardwareSerial  gpsSW(1);
TinyGPSPlus     gps;

// Sensör nesneleri
Adafruit_BME280 bme;

void setup() {
  Serial.begin(115200);
  SerialAT.begin(9600, SERIAL_8N1, LORA_RX, LORA_TX); //lora ve gps
  Wire.begin(BME_SDA, BME_SCL);
  Wire.setClock(400000);

  gpsSW.begin(GPS_BAUD, SERIAL_8N1, GPS_RX, GPS_TX); //lora
  e32ttl.begin(); // lora
  
  // BME280 Başlatma (I2C adresi 0x76 veya 0x77 olabilir)
  if (!bme.begin(0x76)) {
    Serial.println("BME280 sensor not detected.");
  }

  esp_task_wdt_config_t twdt_config = { // task watchdog configuraiton
    .timeout_ms = 3000, // 3 saniye içinde resetlenmeli
    .idle_core_mask = 0, // idle task izlenmiyor
    .trigger_panic = true
  };

  esp_task_wdt_init(&twdt_config); // Watchdog’u başlat
  esp_task_wdt_add(NULL);          // Mevcut görev (loop task) TWDT’ye abone edildi
  
  lora_timer = millis();

  t.once(2.0/*second*/, first_flag_function);
}

void loop() {
  // BME280'dan altimetre ölçümü (deniz seviyesi basıncı 1013.25 hPa)
  altitude = bme.readAltitude(1013.25);
  yukseklik = altitude - yukseklikraw;

  basinc = bme.readPressure();
  sicaklik = bme.readTemperature();
  nem = bme.readHumidity();

  call_lora();  // Sadece tetik alındığında veri gönder

  // if(millis() - lora_timer >= 0){
  //   call_lora();
  //   lora_timer = millis();
  // }

  esp_task_wdt_reset(); // watchdog'u besle
  
}