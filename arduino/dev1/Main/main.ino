#include "boot_screen.h"
#include "gps_sensor.h"
#include "lora_transmitter.h"
#include "compass_sensor.h"

// GPS setup
#define GPS_TX_PIN 33
#define GPS_RX_PIN 32

// LoRa E220 setup
#define LORA_RX 16
#define LORA_TX 17


#define DISPLAY_UPDATE_INTERVAL 500
unsigned long lastDisplayUpdate = 0;
bool lastWaitState = false;


//received location data 
String remoteLatStr = "N/A";
String remoteLonStr = "N/A";
float remoteDistance = 0.0;
float remoteBearing = 0.0; 
bool remoteDataReceived = false;


void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("=== Display initialization ===");
  Serial.println("=== GPS + LoRa Location Transmitter ===");
  Serial.println("Initializing GPS and LoRa modules...");
  // Initialize TFT display
  initBootScreen();
  
  
  // Initialize GPS (Serial1)
  Serial1.begin(9600, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
  Serial.println("GPS module initialized on Serial1");

  if (Serial1.available() > 0) {
    Serial.println("✓ GPS module is ready");
  }
  else {
    Serial.println("✗ GPS module not responding");
  }

  // Initialize LoRa E220 (Serial2)
  E220.begin(9600, SERIAL_8N1, LORA_RX, LORA_TX);
  delay(500);
  Serial.println("LoRa E220 module initialized on Serial2");

  // Initialize Compass Sensor
  Wire.begin();
  if (checkSensor()) {
      if (initSensor()) {
          Serial.println("Compass sensor initialized successfully.");
      } else {
          Serial.println("Failed to initialize compass sensor.");
      }
  } else {
      Serial.println("Compass sensor not found.");
  }

  // Show boot screen
  drawBootScreen();
  delay(2000);
  tft.fillScreen(COLOR_BG);
  
  Serial.println("Starting main loop...");
  Serial.println("=====================================");
}

void loop() {
  unsigned long currentTime = millis();
  
  // ===== GPS DATA READING (Event-driven) =====
  bool newGPSData = false;
  while (Serial1.available() > 0) {
    char c = Serial1.read();
    if (gps.encode(c)) {
      newGPSData = true;
    }
  }
  
  if (newGPSData) {
    Serial.println("✓ GPS module is transmitting data");
    readGPSData();
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

  if (result.isValid){
    // Update remote location data
    remoteLatStr = String(result.latitude, 6);
    remoteLonStr = String(result.longitude, 6);
    remoteDataReceived = true;

    // Only calculate if we have a valid GPS fix
    if (gpsFixed) {
      remoteDistance = calculateDistance(latitude, longitude, result.latitude, result.longitude);
      remoteBearing = calculateBearing(latitude, longitude, result.latitude, result.longitude);
    }
  }
  if (readCompass()){
    calculateHeading();
    smoothHeading();
    // String direction = getDirection();
  }
  // ===== DISPLAY UPDATE =====
  bool waitingState = !gpsFixed || !remoteDataReceived;

  // Only redraw wait screen when state changes
  if (waitingState && (waitingState != lastWaitState)) {
    drawWaitScreen();
    
    if (!gpsFixed && !remoteDataReceived) {
      drawCrossSign(41, 158);
      drawCrossSign(170, 158);
    } else if (!gpsFixed) {
      drawCrossSign(41, 158);
      drawConnectedSign(170, 158);
    } else {
      drawConnectedSign(41, 158);
      drawCrossSign(170, 158);
    }
    lastWaitState = waitingState;
  }

  // Update compass display at controlled rate
  if (!waitingState && (currentTime - lastDisplayUpdate >= DISPLAY_UPDATE_INTERVAL)) {
    drawRotatingCompass(compass.smoothedHeading);
    drawRemoteDevice(remoteBearing, compass.smoothedHeading);
    drawStaticNeedle();
    displayDistance(remoteDistance);
    lastDisplayUpdate = currentTime;
  }

  // Small delay to prevent excessive CPU usage
  // delay(50);
}
 