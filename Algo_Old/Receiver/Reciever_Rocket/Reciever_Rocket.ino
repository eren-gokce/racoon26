#include <Arduino.h>
#include <LoRa_E32.h>
#include <cstring>  // memcpy
#include "driver/uart.h"
#include <math.h>

HardwareSerial SerialAT(2);
HardwareSerial IletiSerial(1);
#define LORA_RX 16
#define LORA_TX 17

#define ILETI_RX 18
#define ILETI_TX 19

#pragma pack(push,1)
struct Rocket {
  uint8_t id;
  float   baroAlt;
  float   gpsAlt;
  float   lat;
  float   lon;
  float   accelX;
  float   accelY;
  float   accelZ;
  float   speed; // ek benim istedigim bilgi
  uint8_t status;
  uint8_t crc;
};
#pragma pack(pop)
#pragma pack(push,1)
struct Ileti {
  float   baroAlt;
  float   gpsAlt;
  float   lat;
  float   lon;
  float   pressure;
  float   temp;
  float   humidity;
};
#pragma pack(pop)

Ileti i;

uint8_t calculateCRC8(const uint8_t* data, size_t len) {
  uint8_t crc = 0;
  for (size_t i = 0; i < len; i++) {
    crc ^= data[i];
    for (uint8_t b = 0; b < 8; b++) {
      if (crc & 0x80)
        crc = (crc << 1) ^ 0x31;
      else
        crc <<= 1;
    }
  }
  return crc;
}

#define BUFFER_SIZE 256
uint8_t fifoBuffer[BUFFER_SIZE];
uint8_t fifoHead = 0;
uint8_t fifoTail = 0;

void fifoPush(uint8_t b) { // fifo ya byte ekleme
  fifoBuffer[fifoHead] = b;
  fifoHead = (fifoHead + 1) % BUFFER_SIZE;

  // FIFO dolarsa, en eski veriyi ez
  if (fifoHead == fifoTail) {
    fifoTail = (fifoTail + 1) % BUFFER_SIZE;
  }
}

bool fifoPop(uint8_t &b) { // fifo byte okuma
  if (fifoHead == fifoTail) return false; // boş
  b = fifoBuffer[fifoTail];
  fifoTail = (fifoTail + 1) % BUFFER_SIZE;
  return true;
}

uint8_t fifoAvailable() {  // fifo veri uzunlugu
  return (fifoHead + BUFFER_SIZE - fifoTail) % BUFFER_SIZE;
}

void parseFifo() {
  if (fifoAvailable() >= sizeof(Rocket)) {
    uint8_t startByte;
    startByte = fifoBuffer[fifoTail];

    if (startByte == 151 && fifoAvailable() >= sizeof(Rocket)) {
      // Rocket mesajı
      Rocket r;
      for (int i = 0; i < sizeof(Rocket); i++) {
        fifoPop(((uint8_t*)&r)[i]);
      }
      
      uint8_t calc_crc = calculateCRC8((uint8_t*)&r, sizeof(Rocket) - sizeof(r.crc));

      if(calc_crc == r.crc){
        // Serial.print("  id: ");        Serial.println(r.id);
        Serial.print("  BaroAlt: ");   Serial.println(r.baroAlt);
        Serial.print("  GPSAlt: ");    Serial.println(r.gpsAlt);
        Serial.print("  Lat: ");       Serial.println(r.lat,6);
        Serial.print("  Lon: ");       Serial.println(r.lon,6);
        Serial.print("  Accel X : ");  Serial.println(r.accelX,3);
        Serial.print("  Accel Y : ");  Serial.println(r.accelY,3);
        Serial.print("  Accel Z : ");  Serial.println(r.accelZ,3);
        Serial.print("  Speed: ");     Serial.println(r.speed);
        Serial.print("  Status: ");    Serial.println(r.status);
      }
    } 
    else {
      // Geçerli mesaj başlangıcı değil, at
      uint8_t trash;
      fifoPop(trash);
      Serial.print("///////////////////////////////////////////////////////////////////////////////////////////////////////// hatali byte: "); Serial.println(trash);
    }
  }
}




  void setup() {
  Serial.begin(115200);
  SerialAT.begin(9600, SERIAL_8N1, LORA_RX, LORA_TX);
  IletiSerial.begin(115200, SERIAL_8N1, ILETI_RX, ILETI_TX);
  while (!Serial);
  delay(100);
  Serial.println("\n🚀 RX: waiting for packets…");
}

void loop() {
  if (SerialAT.available()) {
    uint8_t b = SerialAT.read();
    // Serial.print("RX: "); Serial.println(b);
    fifoPush(b);
  }

  if(IletiSerial.available() >= sizeof(Ileti)){
    IletiSerial.readBytes((uint8_t*)&i, sizeof(Ileti));

    Serial.print("  Payload_BaroAlt:  ");   Serial.println(i.baroAlt);
    Serial.print("  Payload_GPSAlt:  ");    Serial.println(i.gpsAlt);
    Serial.print("  Payload_Lat:  ");       Serial.println(i.lat,6);
    Serial.print("  Payload_Lon:  ");       Serial.println(i.lon,6);
    Serial.print("  Payload_Pressure:  ");  Serial.println(i.pressure);
    Serial.print("  Payload_Temp:  ");      Serial.println(i.temp);
    Serial.print("  Payload_Humidity:  ");  Serial.println(i.humidity);
  }

  parseFifo();

}