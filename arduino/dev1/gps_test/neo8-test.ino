
#include <TinyGPS++.h>

// GPS setup
#define GPS_TX_PIN 33
#define GPS_RX_PIN 32

TinyGPSPlus gps;
unsigned long timestamp;
float lat = 0;
float lng = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("GPS Diagnostic Test Starting...");
  
  // GPS Serial port
  Serial1.begin(9600, SERIAL_8N1, GPS_TX_PIN, GPS_RX_PIN);
  
  Serial.println("Wiring Check:");
  Serial.println("GPS TX -> ESP32 GPIO32 (RX)");
  Serial.println("GPS RX -> ESP32 GPIO33 (TX)");
  Serial.println("GPS VCC -> 3.3V");
  Serial.println("GPS GND -> GND");
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
    Serial.println("Raw GPS data received:");
    // Show raw data for 1 second
    unsigned long start = millis();
    while (millis() - start < 1000) {
      while (Serial1.available() > 0) {
        char c = Serial1.read();
        Serial.print(c); // Print raw NMEA data
        gps.encode(c);
      }
    }
    Serial.println();
    
    // Check GPS status
    Serial.print("Characters processed: ");
    Serial.println(gps.charsProcessed());
    Serial.print("Sentences with fix: ");
    Serial.println(gps.sentencesWithFix());
    Serial.print("Failed checksum: ");
    Serial.println(gps.failedChecksum());
    Serial.print("Satellites: ");
    Serial.println(gps.satellites.value());
    
    if (gps.location.isValid()) {
      lat = gps.location.lat();
      lng = gps.location.lng();
      Serial.print("✓ VALID - Latitude: ");
      Serial.println(lat, 6);
      Serial.print("✓ VALID - Longitude: ");
      Serial.println(lng, 6);
    } else {
      Serial.println("✗ Location data invalid (waiting for GPS fix...)");
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