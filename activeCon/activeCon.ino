#include <Wire.h>
#include "MPU6050_6Axis_MotionApps20.h"
#include <ESP32Servo.h>

#pragma region MPU6050

#define MPU_SDA    22   // MPU6050 SDA → IO4
#define MPU_SCL    23   // MPU6050 SCL → IO5
#define INTERRUPT_PIN 2

float ax;
float ay;
float az;

float yaw;
float pitch;
float roll;

float yawDeg;
float pitchDeg;
float rollDeg;

MPU6050 mpu;

// MPU6050 DMP için
 
volatile bool MPUInterrupt = false;
bool DMPReady = false;
uint8_t devStatus;
uint16_t packetSize;
uint8_t FIFOBuffer[64];

Quaternion q;
VectorFloat gravity;
float ypr[3];  // Yaw, Pitch, Roll

// MPU6050 DMP Interrupt Handler
void DMPDataReady() {
  MPUInterrupt = true;
}
#pragma endregion

#pragma region PID

#define SERVO_PIN 18

// --- PID PARAMETRELERİ (Tuning Gerektirir) ---
float Kp = 2.0;   // Oransal çarpan (Hızlı tepki)
float Ki = 0.01;  // Integral çarpan (Hata birikimi)
float Kd = 1.2;   // Türev çarpan (Sönümleme)

float rollError, lastRollError, rollIntegral, rollDerivative;
float setPoint = 0; // Hedef: 0 derece roll
float pidOutput = 0;

// --- ZAMANLAMA ---
unsigned long lastTime;
const int sampleTime = 100; // 10ms = 100Hz döngü hızı

Servo rollServo;

#pragma endregion

void setup() {
  Serial.begin(115200);
  Wire.begin(MPU_SDA, MPU_SCL);
  Wire.setClock(400000);
  
  #pragma region MPU6050 setup
  Serial.println("Initializing MPU6050...");
  mpu.initialize();
  pinMode(INTERRUPT_PIN, INPUT);
  Serial.println(mpu.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  
  devStatus = mpu.dmpInitialize();
  if (devStatus == 0) {
    mpu.setXAccelOffset(-1600);
    mpu.setYAccelOffset(-2892);
    mpu.setZAccelOffset(1050);
    mpu.setXGyroOffset(138);
    mpu.setYGyroOffset(-17);
    mpu.setZGyroOffset(25);

    mpu.PrintActiveOffsets();
    
    mpu.setDMPEnabled(true);
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), DMPDataReady, RISING);
    Serial.println("DMP ready!");
    DMPReady = true;
    packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
    Serial.print("DMP Initialization failed (code ");
    Serial.print(devStatus);
    Serial.println(")");
  }
  #pragma endregion

  #pragma region PID setup

  // Servo kurulumu
  rollServo.attach(SERVO_PIN);
  rollServo.write(90); // Başlangıçta kanatçıkları nötrle (90 derece)

  #pragma endregion

}

void loop() {
  // MPU6050'dan verileri oku (sadece vertical ivme için)
  if (mpu.dmpGetCurrentFIFOPacket(FIFOBuffer)) { 
    mpu.dmpGetQuaternion(&q, FIFOBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity); // Bu satır eklendi, böylece ypr güncellenir.
  }
  ax = gravity.x;
  ay = gravity.y;
  az = gravity.z;

  yaw = ypr[0];
  pitch = ypr[1];
  roll = ypr[2];

  yawDeg = ypr[0] * 180.0 / PI;
  pitchDeg = ypr[1] * 180.0 / PI;
  rollDeg = ypr[2] * 180.0 / PI;

  // Serial.print("yaw: "); Serial.print(yawDeg);
  // Serial.print("\t pitch: "); Serial.print(pitchDeg);
  // Serial.print("\t roll: "); Serial.println(rollDeg);

  // Serial.print("ax: "); Serial.print(ax);
  // Serial.print("\t ay: "); Serial.print(ay);
  // Serial.print("\t az: "); Serial.print(az);

  // --- PID HESAPLAMA DÖNGÜSÜ ---
  unsigned long now = millis();
  float dt = (float)(now - lastTime) / 1000.0;

  if (dt >= (float)sampleTime / 1000.0) {
    // 1. Hata hesabı
    rollError = setPoint - rollDeg;

    // 2. Integral hesabı (Rüzgar vb. etkiler için)
    rollIntegral += rollError * dt;
    // Integral Windup koruması (Limit koymak iyidir)
    rollIntegral = constrain(rollIntegral, -20, 20);

    // 3. Türev hesabı (Hız değişimi)
    rollDerivative = (rollError - lastRollError) / dt;

    // 4. PID Çıkışı
    pidOutput = (Kp * rollError) + (Ki * rollIntegral) + (Kd * rollDerivative);

    // 5. Servoya Uygula
    // Servonun orta noktası 90'dır. PID çıkışını üzerine ekliyoruz.
    int servoTarget = 90 + pidOutput;
    servoTarget = constrain(servoTarget, 45, 135); // Servonun mekanik limiti
    
    rollServo.write(servoTarget);

    // Değerleri güncelle
    lastRollError = rollError;
    lastTime = now;

    // Debug
    Serial.print("yaw: "); Serial.print(yawDeg);
    Serial.print("\t pitch: "); Serial.print(pitchDeg);
    Serial.print("\t roll: "); Serial.println(rollDeg);

    Serial.print("Roll: "); Serial.print(rollDeg);
    Serial.print(" | PID Out: "); Serial.print(pidOutput);
    Serial.print(" | Servo: "); Serial.println(servoTarget);
  }
}
