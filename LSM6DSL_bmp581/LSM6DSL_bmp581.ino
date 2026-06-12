#include <Wire.h>
#include <Adafruit_LSM6DSL.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP5xx.h"
#define private public
#include <MadgwickAHRS.h>
#undef private

// Your custom ESP32-S3 I2C Pins
#define I2C_SDA 6
#define I2C_SCL 7

#define SEALEVELPRESSURE_HPA (1013.25)

// Create sensor and filter objects
Adafruit_LSM6DSL lsm6dsl;
Adafruit_BMP5xx bmp;
bmp5xx_powermode_t desiredMode = BMP5XX_POWERMODE_NORMAL;
Madgwick filter;

// float gyroBiasX = 0.24, gyroBiasY = -4.24, gyroBiasZ = 0.37;
float gyroBiasX = 0, gyroBiasY = 0, gyroBiasZ = 0;

// Variables to keep track of timing for the filter
unsigned long microsPerReading, microsPrevious;
float sampleRate = 416.0; // 100 Hz sample rate

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10); // Wait for serial monitor to open

  Serial.println("Initializing ESP32-S3 and LSM6DSL...");

  // Initialize the I2C bus with your specific pins (SDA=6, SCL=7)
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(400000);

  // Initialize the sensor
  if (!lsm6dsl.begin_I2C()) {
    Serial.println("Failed to find LSM6DSL chip. Check wiring!");
    while (1) {
      delay(1);
    }
  }
  Serial.println("LSM6DSL Found!");

  if (!bmp.begin(0x46, &Wire)) {
    // For SPI mode (uncomment the line below and comment out the I2C line above):
    // if (!bmp.begin(BMP5XX_CS_PIN, &SPI)) {
    Serial.println(F("Could not find a valid BMP5xx sensor, check wiring!"));
    while (1) delay(1);
  }
  Serial.println(F("BMP5xx found!"));

  // Set the data rates for Accel and Gyro (104 Hz is close to our 100Hz filter rate)
  lsm6dsl.setAccelDataRate(LSM6DS_RATE_416_HZ);
  lsm6dsl.setGyroDataRate(LSM6DS_RATE_416_HZ);

  bmp.setTemperatureOversampling(BMP5XX_OVERSAMPLING_2X);
  bmp.setPressureOversampling(BMP5XX_OVERSAMPLING_16X);
  bmp.setIIRFilterCoeff(BMP5XX_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP5XX_ODR_50_HZ);
  desiredMode = BMP5XX_POWERMODE_NORMAL;
  bmp.setPowerMode(desiredMode);
  bmp.enablePressure(true);
  bmp.configureInterrupt(BMP5XX_INTERRUPT_LATCHED, BMP5XX_INTERRUPT_ACTIVE_HIGH, BMP5XX_INTERRUPT_PUSH_PULL, BMP5XX_INTERRUPT_DATA_READY, true);


  // Serial.println("Calibrating Gyro... DO NOT MOVE BOARD!");
  // int numSamples = 10000;
  // for (int i = 0; i < numSamples; i++) {
  //   sensors_event_t accel, gyro, temp;
  //   lsm6dsl.getEvent(&accel, &gyro, &temp);
    
  //   // Accumulate readings in degrees/s
  //   gyroBiasX += gyro.gyro.x * 57.2958;
  //   gyroBiasY += gyro.gyro.y * 57.2958;
  //   gyroBiasZ += gyro.gyro.z * 57.2958;
  //   delay(5);
  // }
  // // Calculate average bias
  // gyroBiasX /= numSamples;
  // gyroBiasY /= numSamples;
  // gyroBiasZ /= numSamples;
  // Serial.println("Calibration complete.");
  // Serial.println("Calibration Results");
  // Serial.print("gyroBiasx: "); Serial.print(gyroBiasX);
  // Serial.print("gyroBiasy: "); Serial.print(gyroBiasY);
  // Serial.print("gyroBiasZ: "); Serial.println(gyroBiasZ);

  // Initialize the Madgwick filter
  filter.begin(sampleRate);

  // Try 0.5 to 1.5 for much faster snap-to-gravity performance.
  filter.beta = 1;
  
  // Calculate how many microseconds pass between each reading
  microsPerReading = 1000000 / sampleRate;
  microsPrevious = micros();
}

void loop() {
  unsigned long microsNow;

  // Check if it's time to read data and update the filter
  microsNow = micros();
  if (microsNow - microsPrevious >= microsPerReading) {
    
    // Read the new sensor events
    sensors_event_t accel, gyro, temp;
    lsm6dsl.getEvent(&accel, &gyro, &temp);

// 1. Read and convert the raw sensor data first
    float raw_ax = accel.acceleration.x / 9.80665;
    float raw_ay = accel.acceleration.y / 9.80665;
    float raw_az = accel.acceleration.z / 9.80665;

    float raw_gx = (gyro.gyro.x * 57.2958) - gyroBiasX;
    float raw_gy = (gyro.gyro.y * 57.2958) - gyroBiasY;
    float raw_gz = (gyro.gyro.z * 57.2958) - gyroBiasZ;

    // 2. Remap Axes (Rotate 90 degrees so physical Z+ acts as software X+)
    // We pass these variables into the Madgwick filter
    float ax_g = -raw_az;
    float ay_g = raw_ay;
    float az_g = raw_ax;

    float gx_deg = -raw_gz;
    float gy_deg = raw_gy;
    float gz_deg = raw_gx;

    // 3. Update Madgwick Filter with the remapped axes
    filter.updateIMU(gx_deg, gy_deg, gz_deg, ax_g, ay_g, az_g);

    if (abs(gx_deg) < 1) gx_deg = 0.0;
    if (abs(gy_deg) < 1) gy_deg = 0.0;
    if (abs(gz_deg) < 1) gz_deg = 0.0;

    // Update the sensor fusion algorithm
    filter.updateIMU(gx_deg, gy_deg, gz_deg, ax_g, ay_g, az_g);

    // Extract the Euler angles
    float roll = filter.getRoll();
    float pitch = filter.getPitch();
    float yaw = filter.getYaw();

    // // Print the results to the Serial Plotter/Monitor
    // // Printing Acceleration in G's
    // Serial.print("AccX:");
    // Serial.print(ax_g);
    // Serial.print(" AccY:");
    // Serial.print(ay_g);
    // Serial.print(" AccZ:");
    // Serial.print(az_g);

    // // Printing Euler Angles
    // Serial.print(" Roll:");
    // Serial.print(roll);
    // Serial.print(" Pitch:");
    // Serial.print(pitch);
    // Serial.print(" Yaw:");
    // Serial.println(yaw); // The last item gets the println to start a new line

    Serial.print("Orientation: ");
    Serial.print(yaw);
    Serial.print(", ");
    Serial.print(pitch);
    Serial.print(", ");
    Serial.println(roll);

    Serial.print(F("Approx. Altitude = "));
    Serial.print(bmp.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(F(" m"));

    // Update timer
    microsPrevious += microsPerReading;
  }
}