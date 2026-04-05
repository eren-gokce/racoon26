#include <Wire.h>
#include <Adafruit_BMP280.h>
#include "MPU6050_6Axis_MotionApps20.h"
#include "functions.h"
#include <cstring>


float accel_x;
float accel_y;
float accel_z;

float sut_yukseklik;
float sut_ivme;

float yukseklikoldold;
float yukseklikold;

uint8_t sut_flag = 0;

float checksum = 0; //degistirilcek

// Komut setleri (Tablo 1)
const uint8_t CMD_SIT_BASLAT[] = {0xAA, 0x20, 0xCA, 0x0D, 0x0A};
const uint8_t CMD_SUT_BASLAT[] = {0xAA, 0x22, 0xCC, 0x0D, 0x0A};
const uint8_t CMD_DURDUR[]     = {0xAA, 0x24, 0xCE, 0x0D, 0x0A}; // Sadece Durdur komutu

uint8_t data1_byte = 0; // sut bit gonderme byte i

#pragma region fifo
uint8_t fifoBuffer[BUFFER_SIZE]; // buffer size is 256 and defined in .h
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
#pragma endregion

uint8_t calculateChecksum(const uint8_t* data, size_t length) {
  uint16_t sum = 0;
  for (size_t i = 0; i < length; i++) {
    sum += data[i];
  }
  return (uint8_t)(0x100 - (sum & 0xFF));  // Toplam + checksum ≡ 0 mod 256
}

float readFloatLE(uint8_t* data) { //degistirildi tersine
  uint8_t temp[4];
  temp[0] = data[3];
  temp[1] = data[2];
  temp[2] = data[1];
  temp[3] = data[0];
  float result;
  memcpy(&result, temp, sizeof(result));
  return result;
}

void parseFifo() {

  if (fifoAvailable() >= 5) {
    uint8_t bytes[4];
    bytes[0] = fifoBuffer[fifoTail];
    if (bytes[0] == 0xAA) {
      for(int i = 1; i < 5; i++) {
        bytes[i] = fifoBuffer[fifoTail + i];
        // fifoPop(bytes[i]);
      }
      if (memcmp(bytes, CMD_DURDUR, 5) == 0) {
        scenario = 0;
        for(int i = 0; i < 5; i++){
          fifoPop(bytes[i]);
        }
        data1_byte = 0;
        sut_flag = 0;
        digitalWrite(SURUKLENME_PARASUT_PIN, LOW);
        digitalWrite(ANA_PARASUT_PIN, LOW);

        yukseklikoldold = 0;
        yukseklikold = 0;
        yukseklik = 0;
        ivme = 0;
        sut_yukseklik = 0;
        sut_ivme = 0;
      }
    }
  }

  if (fifoAvailable() >= 36) {
    uint8_t startByte = fifoBuffer[fifoTail];

    if (startByte == 171 && fifoAvailable() >= 36) {
      uint8_t raw[36];
      for (int i = 0; i < 36; i++) {
        fifoPop(raw[i]);
      }
      float sut_irtifa = readFloatLE(&raw[1]);
      float sut_basinc = readFloatLE(&raw[5]);
      float sut_ivmex  = readFloatLE(&raw[9]);
      float sut_ivmey  = readFloatLE(&raw[13]);
      float sut_ivmez  = readFloatLE(&raw[17]);
      float sut_acix   = readFloatLE(&raw[21]);
      float sut_aciy   = readFloatLE(&raw[25]);
      float sut_aciz   = readFloatLE(&raw[29]);
      float sut_checksum = raw[33];

      uint8_t calc_checksum = calculateChecksum(raw, 33);

      Serial.print("  irtifa: "); Serial.println(sut_irtifa);
      Serial.print("  basinc: "); Serial.println(sut_basinc);
      Serial.print("  ivmex: ");  Serial.println(sut_ivmex);
      Serial.print("  ivmey: ");  Serial.println(sut_ivmey);
      Serial.print("  ivmez: ");  Serial.println(sut_ivmez);
      Serial.print("  acix: ");   Serial.println(sut_acix);
      Serial.print("  aciy: ");   Serial.println(sut_aciy);
      Serial.print("  aciz: ");   Serial.println(sut_aciz);
      Serial.print("  checksum: "); Serial.println(sut_checksum);
      Serial.print("  calc_checksum: "); Serial.println(calc_checksum);

      sut_yukseklik = sut_irtifa;
      sut_ivme = sut_ivmez;

      // if(sut_yukseklik > 300 || sut_irtifa > 300){
      //   digitalWrite(SURUKLENME_PARASUT_PIN, HIGH);
      // }
      // if(sut_ivme > 30){
      //   digitalWrite(ANA_PARASUT_PIN, HIGH);
      // }

      // if (checksum == calc_checksum) {
      //   digitalWrite(SURUKLENME_ _PIN, HIGH);
      //   Serial.println("  checksum: Dogru");
      // } else {
      //   Serial.println("  checksum: Yanlis");
      // }

    } else {
      uint8_t trash;
      fifoPop(trash);
      Serial.print("Hatalı byte: "); Serial.println(trash);
    }
  }
}

uint8_t sut_paket[6];

void setBit(uint8_t n) {
  data1_byte |= (1 << n);  // sadece n. biti 1 yap
}

void send_sut_bytes(){
  sut_paket[0] = 0xAA; //header
  sut_paket[1] = data1_byte; // data1
  sut_paket[2] = 0x00; // data2
  sut_paket[3] = calculateChecksum((uint8_t*)sut_paket, 3); // checksum
  sut_paket[4] = 0x0D; // footer1
  sut_paket[5] = 0x0A; // footer2
  RS232Serial.write(sut_paket, 6);
}

#pragma region rs232 okuma

volatile uint8_t scenario;


// UKB Durum Bayrakları
bool sit_aktif = false; // Sadece SİT aktif bayrağı kullanılıyor
bool sut_aktif = false;
bool stop_kodonu = false;


uint8_t header[5]; // FIFO'dan çekilecek dizi
void checkFifoHeader() {
  // En az 5 byte var mı kontrol et
  if (fifoAvailable() >= 5) {
    uint8_t first;
    fifoPop(first);

    if (first == 0xAA) {
      header[0] = first;

      // 4 byte daha oku
      for (int i = 1; i < 5; i++) {
        fifoPop(header[i]);
      }
    }

    if (memcmp(header, CMD_SIT_BASLAT, 5) == 0) {
      scenario = 1;
      // digitalWrite(ANA_PARASUT_PIN, HIGH);
      delay(100);
    }
    if (memcmp(header, CMD_SUT_BASLAT, 5) == 0) {
      scenario = 2;
      // digitalWrite(SURUKLENME_PARASUT_PIN, HIGH);
      data1_byte = 0;
      sut_flag = 0;
      digitalWrite(SURUKLENME_PARASUT_PIN, LOW);
      digitalWrite(ANA_PARASUT_PIN, LOW);

      yukseklikoldold = 0;
      yukseklikold = 0;
      yukseklik = 0;
      ivme = 0;

      delay(100);
    }
    if (memcmp(header, CMD_DURDUR, 5) == 0) {
      scenario = 0;
      // digitalWrite(SURUKLENME_PARASUT_PIN, LOW);
      // digitalWrite(ANA_PARASUT_PIN, LOW);
      delay(100);
    }
  }
}

#pragma endregion

uint8_t sit_paket[36];

float ikiBasamak(float deger) {
  return round(deger * 100.0f) / 100.0f;
}

uint8_t byte_array[4] = {0};

void donustur(float *deger) {
	byte_donustur a;
	a.sayi = *deger;
	byte_array[0] = a.array[0];
	byte_array[1] = a.array[1];
	byte_array[2] = a.array[2];
	byte_array[3] = a.array[3];
}

void sit(){
  // Serial.println("sit hesapliyor");
  sit_paket[0] = 171;  //0xAB

  float irtifa = ikiBasamak(bmp.readAltitude(1013.25));
  donustur(&irtifa);
  sit_paket[1] = byte_array[3];
  sit_paket[2] = byte_array[2];
  sit_paket[3] = byte_array[1];
  sit_paket[4] = byte_array[0];

  float basinc = ikiBasamak(bmp.readPressure() / 100.0F);
  donustur(&basinc);
  sit_paket[5] = byte_array[3];
  sit_paket[6] = byte_array[2];
  sit_paket[7] = byte_array[1];
  sit_paket[8] = byte_array[0];

  float ivmeX = ikiBasamak(accel_x);
  donustur(&ivmeX);
  sit_paket[9] = byte_array[3];
  sit_paket[10] = byte_array[2];
  sit_paket[11] = byte_array[1];
  sit_paket[12] = byte_array[0];

  float ivmeY = ikiBasamak(accel_y);
  donustur(&ivmeY);
  sit_paket[13] = byte_array[3];
  sit_paket[14] = byte_array[2];
  sit_paket[15] = byte_array[1];
  sit_paket[16] = byte_array[0];

  float ivmeZ = ikiBasamak(ivme);
  donustur(&ivmeZ);
  sit_paket[17] = byte_array[3];
  sit_paket[18] = byte_array[2];
  sit_paket[19] = byte_array[1];
  sit_paket[20] = byte_array[0];

  float aciX = ikiBasamak(pitchDeg);
  donustur(&aciX);
  sit_paket[21] = byte_array[3];
  sit_paket[22] = byte_array[2];
  sit_paket[23] = byte_array[1];
  sit_paket[24] = byte_array[0];

  float aciY = ikiBasamak(rollDeg);
  donustur(&aciY);
  sit_paket[25] = byte_array[3];
  sit_paket[26] = byte_array[2];
  sit_paket[27] = byte_array[1];
  sit_paket[28] = byte_array[0];

  float aciZ = ikiBasamak(yawDeg);
  donustur(&aciZ);
  sit_paket[29] = byte_array[3];
  sit_paket[30] = byte_array[2];
  sit_paket[31] = byte_array[1];
  sit_paket[32] = byte_array[0];
  

  sit_paket[33] = calculateChecksum((uint8_t*)sit_paket, 33);
  sit_paket[34] = 0x0D;
  sit_paket[35] = 0x0A;

  // for(int i = 0; i < 36; i++){
  //   Serial.println(sit_paket[i]);
  // }

  RS232Serial.write(sit_paket, 36);
  Serial.write(sit_paket, 36);
  delay(60);
} 
float sut_millis_start = 0;
float sut_millis;
void sut(){
  if (RS232Serial.available()) {
    uint8_t b = RS232Serial.read();
    // if(b > 0){
    //   digitalWrite(ANA_PARASUT_PIN, HIGH);
    //   delay(100);
    //   digitalWrite(ANA_PARASUT_PIN, LOW);
    // }
    fifoPush(b);
  }
  parseFifo();
  sut_millis = millis() - sut_millis_start;
  if(sut_millis > 500){
    send_sut_bytes();
    sut_millis_start = millis();
  }
}