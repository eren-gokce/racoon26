//lora.cpp
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include "MPU6050_6Axis_MotionApps20.h"
#include "functions.h"
#include <LoRa_E32.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <SPI.h>
#include <SD.h>

Payload p;
Payload s;

static uint8_t packetCounter = 0;

uint8_t status;

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

void call_lora(){
  if(flag != 254){
    lora_loop();
  }
}

void lora_loop(){
  unsigned long t0 = millis();
  //lora_time_passed = millis() - lora_millis_saved;
  //if(lora_time_passed >= 400){

  while (gpsSW.available()) {
    gps.encode(gpsSW.read());
  }

  static float lastGpsAlt = 0.0f;

  if (gps.altitude.isUpdated()) {
    lastGpsAlt = gps.altitude.meters();
  }

  s.id  = 151;                 
  s.baroAlt = yukseklik;
  s.gpsAlt  = lastGpsAlt;
  s.lat     = gps.location.isValid() ? gps.location.lat() : 0;
  s.lon     = gps.location.isValid() ? gps.location.lng() : 0;
  s.accelX  = accel_x;
  s.accelY  = accel_y;
  s.accelZ  = ivme;
  s.speed   = hiz;
  s.status  = status;  //flagler yazilcak;
  s.crc     = calculateCRC8((uint8_t*)&p, sizeof(Payload) - sizeof(s.crc));

  // ▶️▶️ Debug TX:
  Serial.print("  id: ");   Serial.println(s.id);

  Serial.print("  BaroAlt: ");   Serial.println(s.baroAlt);
  Serial.print("  GPSAlt: ");    Serial.println(s.gpsAlt);

  Serial.print("  Lat: ");       Serial.println(s.lat,6);
  Serial.print("  Lon: ");       Serial.println(s.lon,6);

  Serial.print("  Accel X : ");  Serial.print(s.accelX,3);
  Serial.print("  Accel Y : ");  Serial.print(s.accelY,3);
  Serial.print("  Accel Z : ");  Serial.println(s.accelZ,3);

  Serial.print("  Speed: ");       Serial.println(s.speed);

  Serial.print("  Status: ");    Serial.println(s.status);
  Serial.print("  CRC:  ");     Serial.println(s.crc);

  // LoRa ile gönder
  ResponseStatus rs = e32ttl.sendFixedMessage(
    0, 40, 23,
    reinterpret_cast<uint8_t*>(&p),
    sizeof(p)
  );
  // Serial.print("LoRa: "); Serial.println(rs.getResponseDescription());
}