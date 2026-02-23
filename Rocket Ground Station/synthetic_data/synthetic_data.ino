
uint8_t calculateCRC8(const uint8_t* data, size_t len);

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
  float   gyroX;
  float   gyroY;
  float   gyroZ;
  float   speed;
  uint8_t status;
  uint8_t crc;
};
#pragma pack(pop)


Payload packet;

void setup() {
  Serial.begin(115200);
}

void loop() {
  packet.id = 151;
  packet.baroAlt = 960 + random(1000)/1000.0;
  packet.gpsAlt = 960 + random(1000)/1000.0;
  packet.lat = 39.96 + random(100)/1000.0;
  packet.lon = 32.74 + random(100)/1000.0;
  packet.accelX = 0 + random(100)/1000.0;
  packet.accelY = 0 + random(100)/1000.0;
  packet.accelZ = 0 + random(100)/1000.0;
  packet.gyroX = 0 + random(10)/1000.0;
  packet.gyroY = 0 + random(10)/1000.0;
  packet.gyroZ = 0 + random(10)/1000.0;
  packet.speed = 0 + random(100)/1000.0;
  packet.status = 1;
  packet.crc= calculateCRC8((uint8_t*)&packet, sizeof(packet) - sizeof(packet.crc));

  Serial.write((const uint8_t*)&packet, sizeof(packet));

  delay(200);
} 

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
