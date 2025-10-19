#include <TinyGPS++.h>
#include <HardwareSerial.h>

// GPS setup
#define GPS_TX_PIN 33
#define GPS_RX_PIN 32

// LoRa E220 setup
#define LORA_RX 16
#define LORA_TX 17

// Objects
TinyGPSPlus gps;
HardwareSerial E220(2); // Use UART2 for LoRa

// GPS data variables
float latitude = 0.0;
float longitude = 0.0;
bool gpsFixed = false;

// Timing variables
unsigned long lastGPSRead = 0;
unsigned long lastLoRaTransmit = 0;
const unsigned long GPS_READ_INTERVAL = 500;    // Read GPS every 0.5 seconds
const unsigned long LORA_TRANSMIT_INTERVAL = 5000; // Transmit every 5 seconds

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("=== GPS + LoRa Location Transmitter ===");
  Serial.println("Initializing GPS and LoRa modules...");
  
  // Initialize GPS (Serial1)
  Serial1.begin(9600, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
  Serial.println("GPS module initialized on Serial1");
  
  // Initialize LoRa E220 (Serial2)
  E220.begin(9600, SERIAL_8N1, LORA_RX, LORA_TX);
  delay(500);
  Serial.println("LoRa E220 module initialized on Serial2");
  
  // Serial.println("Wiring Guide:");
  // Serial.println("GPS Module:");
  // Serial.println("  GPS TX -> ESP32 GPIO32");
  // Serial.println("  GPS RX -> ESP32 GPIO33");
  // Serial.println("  GPS VCC -> 3.3V");
  // Serial.println("  GPS GND -> GND");
  // Serial.println("LoRa E220 Module:");
  // Serial.println("  E220 TX -> ESP32 GPIO16");
  // Serial.println("  E220 RX -> ESP32 GPIO17");
  // Serial.println("  E220 VCC -> 3.3V");
  // Serial.println("  E220 GND -> GND");
  // Serial.println();
  Serial.println("Starting main loop...");
  Serial.println("=====================================");
}

void loop() {
  unsigned long currentTime = millis();
  
  // Read GPS data
  if (currentTime - lastGPSRead >= GPS_READ_INTERVAL) {
    readGPSData();
    lastGPSRead = currentTime;
  }
  
  // Transmit location via LoRa if GPS has a fix
  if (currentTime - lastLoRaTransmit >= LORA_TRANSMIT_INTERVAL) {
    if (gpsFixed) {
      transmitLocationData();
    } else {
      Serial.println("⏳ Waiting for GPS fix before transmitting...");
    }
    lastLoRaTransmit = currentTime;
  }
  
  // Check for incoming LoRa messages
  checkIncomingMessages();
  
  // Small delay to prevent excessive CPU usage
  delay(100);
}

void readGPSData() {
  // Process GPS data
  while (Serial1.available() > 0) {
    char c = Serial1.read();
    if (gps.encode(c)) {
      // New sentence completed
      if (gps.location.isValid()) {
        latitude = gps.location.lat();
        longitude = gps.location.lng();
        gpsFixed = true;
        
        // Print GPS status (less verbose than test version)
        Serial.print("📍 GPS: ");
        Serial.print(latitude, 6);
        Serial.print(", ");
        Serial.print(longitude, 6);
        Serial.print(" | Sats: ");
        Serial.print(gps.satellites.value());
        Serial.print(" | Accuracy: ");
        Serial.print(gps.hdop.hdop(), 1);
        Serial.println(" HDOP");
      } else {
        gpsFixed = false;
      }
    }
  }
}

void transmitLocationData() {
  // Create location message in JSON-like format for easy parsing
  String locationMessage = "LOC:";
  locationMessage += String(latitude, 6);
  locationMessage += ",";
  locationMessage += String(longitude, 6);
  locationMessage += ",";
  locationMessage += String(gps.satellites.value());
  locationMessage += ",";
  locationMessage += String(gps.hdop.hdop(), 1);
  
  // Send via LoRa
  E220.println(locationMessage);
  
  // Log to serial
  Serial.print("📡 Transmitted: ");
  Serial.println(locationMessage);
  Serial.print("   Format: LOC:latitude,longitude,satellites,hdop");
  Serial.println();
}

void checkIncomingMessages() {
  if (E220.available()) {
    String receivedMessage = E220.readStringUntil('\n');
    receivedMessage.trim(); // Remove any trailing whitespace
    
    Serial.print("📨 Received: ");
    Serial.println(receivedMessage);
    
    // Parse incoming location messages
    if (receivedMessage.startsWith("LOC:")) {
      parseLocationMessage(receivedMessage);
    }
  }
}

void parseLocationMessage(String message) {
  // Remove "LOC:" prefix
  String data = message.substring(4);
  
  // Parse comma-separated values
  int firstComma = data.indexOf(',');
  int secondComma = data.indexOf(',', firstComma + 1);
  int thirdComma = data.indexOf(',', secondComma + 1);
  
  if (firstComma > 0 && secondComma > 0 && thirdComma > 0) {
    float remoteLatitude = data.substring(0, firstComma).toFloat();
    float remoteLongitude = data.substring(firstComma + 1, secondComma).toFloat();
    int remoteSatellites = data.substring(secondComma + 1, thirdComma).toInt();
    float remoteHDOP = data.substring(thirdComma + 1).toFloat();
    
    Serial.println("📍 Remote Location:");
    Serial.print("   Lat: ");
    Serial.print(remoteLatitude, 6);
    Serial.print(" | Lng: ");
    Serial.print(remoteLongitude, 6);
    Serial.print(" | Sats: ");
    Serial.print(remoteSatellites);
    Serial.print(" | HDOP: ");
    Serial.println(remoteHDOP, 1);
    
    // Calculate distance if we have our own GPS fix
    if (gpsFixed) {
      double distance = calculateDistance(latitude, longitude, remoteLatitude, remoteLongitude);
      Serial.print("📏 Distance: ");
      Serial.print(distance, 2);
      Serial.println(" km");
    }
    Serial.println();
  }
}

double calculateDistance(double lat1, double lon1, double lat2, double lon2) {
  // Haversine formula to calculate distance between two points on Earth
  const double R = 6371.0; // Earth's radius in kilometers
  
  double lat1Rad = lat1 * PI / 180.0;
  double lon1Rad = lon1 * PI / 180.0;
  double lat2Rad = lat2 * PI / 180.0;
  double lon2Rad = lon2 * PI / 180.0;
  
  double dLat = lat2Rad - lat1Rad;
  double dLon = lon2Rad - lon1Rad;
  
  double a = sin(dLat / 2) * sin(dLat / 2) + cos(lat1Rad) * cos(lat2Rad) * sin(dLon / 2) * sin(dLon / 2);
  double c = 2 * atan2(sqrt(a), sqrt(1 - a));
  
  return R * c;
}
