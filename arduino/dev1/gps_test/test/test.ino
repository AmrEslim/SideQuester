#include <TinyGPS++.h>

// GPS setup
#define GPS_RX_PIN 32  // ESP32 RX pin (connects to GPS TX)
#define GPS_TX_PIN 33  // ESP32 TX pin (connects to GPS RX)

TinyGPSPlus gps;
unsigned long timestamp;
float lat = 0;
float lng = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("GPS Diagnostic Test Starting...");
  
  // GPS Serial port - 115200 baud (as confirmed by your test)
  Serial1.begin(115200, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
  
  Serial.println("Wiring Check:");
  Serial.println("GPS TX -> ESP32 GPIO32 (RX)");
  Serial.println("GPS RX -> ESP32 GPIO33 (TX)");
  Serial.println("GPS VCC -> 3.3V");
  Serial.println("GPS GND -> GND");
  Serial.println();
  Serial.println("⚠️  IMPORTANT: GPS must be OUTSIDE with clear sky view!");
  Serial.println("    Cold start can take 30 seconds to 5 minutes.");
  Serial.println();
}

void loop() {
  get_gps_data();
  delay(2000);
}

void get_gps_data() {
  Serial.println("=== GPS Debug Info ===");
  
  // Check if ANY data is coming from GPS
  int bytesAvailable = Serial1.available();
  Serial.print("Bytes available: ");
  Serial.println(bytesAvailable);
  
  if (bytesAvailable > 0) {
    Serial.println("✓ GPS module is transmitting data");
    
    // Process data for 1 second
    unsigned long start = millis();
    while (millis() - start < 1000) {
      while (Serial1.available() > 0) {
        char c = Serial1.read();
        gps.encode(c);
      }
    }
    
    // Check GPS status
    Serial.print("Characters processed: ");
    Serial.println(gps.charsProcessed());
    Serial.print("Sentences with fix: ");
    Serial.println(gps.sentencesWithFix());
    Serial.print("Failed checksum: ");
    Serial.println(gps.failedChecksum());
    Serial.print("Satellites in view: ");
    Serial.println(gps.satellites.value());
    
    if (gps.location.isValid()) {
      lat = gps.location.lat();
      lng = gps.location.lng();
      Serial.print("✓ VALID - Latitude: ");
      Serial.println(lat, 6);
      Serial.print("✓ VALID - Longitude: ");
      Serial.println(lng, 6);
      Serial.print("Altitude: ");
      Serial.print(gps.altitude.meters());
      Serial.println(" m");
      Serial.print("Speed: ");
      Serial.print(gps.speed.kmph());
      Serial.println(" km/h");
    } else {
      Serial.println("✗ Location data invalid");
      if (gps.satellites.value() == 0) {
        Serial.println("   → No satellites visible - GO OUTSIDE!");
      } else {
        Serial.println("   → Satellites visible but waiting for fix...");
      }
    }
    
    // Show time data if available
    if (gps.time.isValid()) {
      Serial.print("GPS Time: ");
      Serial.print(gps.time.hour());
      Serial.print(":");
      Serial.print(gps.time.minute());
      Serial.print(":");
      Serial.println(gps.time.second());
    }
    
  } else {
    Serial.println("✗ NO DATA from GPS module!");
    Serial.println("Check:");
    Serial.println("1. Wiring connections");
    Serial.println("2. GPS module power (3.3V)");
    Serial.println("3. GPS module is working");
  }
  
  Serial.println("========================");
  Serial.println();
}