//functions.h
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <Wire.h>
#include <Adafruit_BME280.h>
#include "functions.h"
#include <LoRa_E32.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

/*
Pins
bme sda 20
bme scl 19

neo8m tx 38
neo8m rx 39

lora tx 42
lora rx 41
*/

#define BME_SDA 20
#define BME_SCL 19

#define GPS_RX 1
#define GPS_TX 2
#define GPS_BAUD 9600

#define LORA_RX 41
#define LORA_TX 42

#pragma region lora

void call_lora();
//void lora_counter();
uint8_t calculateCRC8(const uint8_t* data, size_t len);

extern HardwareSerial  SerialAT;
extern LoRa_E32        e32ttl;
extern HardwareSerial  gpsSW;
extern TinyGPSPlus     gps;


#pragma pack(push,1)
struct Payload {
  uint8_t id;
  float   baroAlt;
  float   gpsAlt;
  float   lat;
  float   lon;
  float   pressure;
  float   temp;
  float   humidity;
  uint8_t crc;
};
#pragma pack(pop)
#pragma endregion

void lora_loop();

#pragma region externs

extern float yukseklikraw;
extern float yukseklik;
extern float sicaklik;
extern float basinc;
extern float nem;

#pragma endregion

#endif