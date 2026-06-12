#include <Wire.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>

// Define your specific pins here
#define GPS_SDA 18
#define GPS_SCL 17

SFE_UBLOX_GNSS myGNSS;

void setup() {
  Serial.begin(115200);

  // Initialize the specific pins for the I2C bus
  Wire.begin(GPS_SDA, GPS_SCL);
  Wire.setClock(400000);

  // Initialize the GNSS
  if (myGNSS.begin(Wire) == false) {
    Serial.println(F("GNSS not detected at default I2C address."));
    while (1);
  }
  myGNSS.setI2COutput(COM_TYPE_UBX); // Ensure UBX is enabled on I2C

  if (myGNSS.setNavigationFrequency(10)) { 
    Serial.println(F("Navigation frequency set to 10Hz!"));
  } else {
    Serial.println(F("Failed to set navigation frequency."));
  }

  Serial.println(F("GNSS initialised!"));
}

void loop() {
  // getPVT() returns true if a new navigation solution has been received
  if (myGNSS.getPVT()) {
    double lat = myGNSS.getLatitude() / 10000000.0;
    double lon = myGNSS.getLongitude() / 10000000.0;
    double alt = myGNSS.getAltitude() / 1000.0;

    Serial.print("Lat: "); Serial.print(lat,7);
    Serial.print("  Lon: "); Serial.print(lon,7);
    Serial.print("  Alt: "); Serial.println(alt);
  }
}