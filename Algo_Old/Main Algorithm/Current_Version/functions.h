//functions.h
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <Wire.h>
#include <Adafruit_BMP280.h>
#include "MPU6050_6Axis_MotionApps20.h"
#include "functions.h"
#include <LoRa_E32.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <SPI.h>
#include <SD.h>

#define SURUKLENME_PARASUT_PIN 19
#define ANA_PARASUT_PIN 38

#define MPU_SDA    9   // MPU6050 SDA → IO4
#define MPU_SCL    10   // MPU6050 SCL → IO5

#define BMP_SDA   7   // BMP280 SDA → IO21
#define BMP_SCL   6   // BMP280 SCL → IO22
#define BMP_ADDR 0x76  // BMP280 adresi

#define CS_PIN   12
#define SCK_PIN  13
#define MOSI_PIN 14
#define MISO_PIN 21

#pragma region lora

void call_lora();
//void lora_counter();

extern HardwareSerial  SerialAT;
extern LoRa_E32        e32ttl;
extern HardwareSerial  gpsSW;
extern TinyGPSPlus     gps;


#define LORA_RX     40
#define LORA_TX     39
#define GPS_RX_PIN  42
#define GPS_TX_PIN  41
#define GPS_BAUD    9600

#pragma pack(push,1)
struct Payload {
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
#pragma endregion

extern Payload p;

void kalkis();
void burnout();
void apogee();
void parasut_tekrarla(uint8_t x);
void parasut();
void alcalma();

void write_to_sd();

extern bool SD_status;
extern const char *filename;
extern File dataFile;

float avarage_in_given_time(unsigned long given_time /*as ms*/, float sensor_value);
extern float son_ortalama;

void lora_loop();

extern float yukseklikoldold;
extern float yukseklikold;

extern Adafruit_BMP280 bmp;

#pragma region externs
//flag
extern volatile uint8_t flag;
// m_n lerin değerleri
extern float yukseklikraw; //m_n[0]
extern float yukseklik; // m_n[0] - yukseklikraw
extern float hiz; // m_n[1]
extern float ivme; // m_n[2]
//ypr değerleri
extern float pitch;
extern float roll;
extern float yaw;
//ypr derece degerleri
extern float yawDeg;
extern float pitchDeg;
extern float rollDeg;
//avarage_calc icin millis
extern unsigned long millis_saved;
extern unsigned long millis_counter;

extern uint8_t status;

//sitsut
extern float accel_x;
extern float accel_y;
extern float accel_z;

#pragma endregion

#endif